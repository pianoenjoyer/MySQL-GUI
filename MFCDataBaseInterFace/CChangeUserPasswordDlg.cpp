// CChangeUserPasswordDlg.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CChangeUserPasswordDlg.h"
#include "resource.h"
#include "algorithm"
#include "Convertions.h"
// CChangeUserPasswordDlg dialog

IMPLEMENT_DYNAMIC(CChangeUserPasswordDlg, CDialogEx)

CChangeUserPasswordDlg::CChangeUserPasswordDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHANGEUSRPWD, pParent)
{

}

CChangeUserPasswordDlg::CChangeUserPasswordDlg(CWnd* pParent /*=nullptr*/, std::shared_ptr<CDBConnection> db)
	: CDialogEx(IDD_CHANGEUSRPWD, pParent) , db(db)
{

}


CChangeUserPasswordDlg::~CChangeUserPasswordDlg()
{
}

void CChangeUserPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChangeUserPasswordDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_GENERATE, &CChangeUserPasswordDlg::OnBnClickedBtnGenerate)
    ON_EN_CHANGE(IDC_EDIT_ENTERPASSWORD, &CChangeUserPasswordDlg::OnEnChangeEditEnterpassword)
    ON_BN_CLICKED(IDC_CANCEL, &CChangeUserPasswordDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_CHANGEPWD, &CChangeUserPasswordDlg::OnBnClickedChangepwd)
    ON_BN_CLICKED(IDC_RADIO_NOPASSWORD, &CChangeUserPasswordDlg::OnBnClickedRadioNopassword)
    ON_BN_CLICKED(IDC_RADIO_PASSWORD, &CChangeUserPasswordDlg::OnBnClickedRadioPassword)
END_MESSAGE_MAP()


// CChangeUserPasswordDlg message handlers
BOOL CChangeUserPasswordDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
    auto pRadioPassword = (CButton*)GetDlgItem(IDC_RADIO_PASSWORD);
    if (pRadioPassword)
    {
        pRadioPassword->SetCheck(BST_CHECKED);
    }


	return TRUE;
}



void CChangeUserPasswordDlg::OnBnClickedBtnGenerate()
{
    // Define characters for the password
    const std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
    const std::string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string numbers = "0123456789";
    const std::string specialChars = "!@#$%^&*()-_=+[]{}|;:'\",.<>?/";

    // Combine all characters
    const std::string allChars = lowercase + uppercase + numbers + specialChars;

    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Set the length of the password
    const int passwordLength = 12;

    // Shuffle the characters
    std::string shuffledChars = allChars;
    std::random_shuffle(shuffledChars.begin(), shuffledChars.end());

    // Choose at least one character from each category
    char password[passwordLength];
    password[0] = lowercase[rand() % lowercase.length()];
    password[1] = uppercase[rand() % uppercase.length()];
    password[2] = numbers[rand() % numbers.length()];
    password[3] = specialChars[rand() % specialChars.length()];

    // Choose the rest of the characters randomly
    for (int i = 4; i < passwordLength; ++i)
    {
        password[i] = shuffledChars[rand() % shuffledChars.length()];
    }

    // Shuffle the entire password
    std::random_shuffle(password, password + passwordLength);

    // Set the generated password in the edit control
    CString generatedPassword(password);
    GetDlgItem(IDC_EDIT_GENPASSWORD)->SetWindowTextW(generatedPassword);
    GetDlgItem(IDC_EDIT_ENTERPASSWORD)->SetWindowTextW(generatedPassword);
    GetDlgItem(IDC_EDIT_RETYPE)->SetWindowTextW(generatedPassword);
}

#include <algorithm>

void CChangeUserPasswordDlg::CheckPasswordStrength(CString password)
{
    auto pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_STRENGTH);
    auto pStrengthText = (CStatic*)GetDlgItem(IDC_STATIC_STRENGTH);

    int passwordLength = password.GetLength();

    // Check for consecutive characters
    bool hasConsecutiveChars = false;
    for (int i = 0; i < passwordLength - 1; ++i)
    {
        if (password[i] + 1 == password[i + 1] || password[i] - 1 == password[i + 1])
        {
            hasConsecutiveChars = true;
            break;
        }
    }

    // Check for frequent repetitions
    bool hasFrequentRepetitions = false;
    std::map<TCHAR, int> charCount;
    for (int i = 0; i < passwordLength; ++i)
    {
        charCount[password[i]]++;
        if (charCount[password[i]] >= passwordLength / 3)  // Adjust the threshold as needed
        {
            hasFrequentRepetitions = true;
            break;
        }
    }

    int strength = 0;

    if (passwordLength < 4)
    {
        // Very weak password
        strength = 0;
    }
    else if (passwordLength < 8)
    {
        // Weak password
        strength = 25;
    }
    else if (passwordLength < 12)
    {
        // Moderate password
        strength = 50;
    }
    else
    {
        // Strong password
        strength = 100;
    }

    // Adjust strength based on additional criteria
    if (hasConsecutiveChars)
    {
        strength -= 10;
    }

    if (hasFrequentRepetitions)
    {
        strength -= 15;
    }

    // Clamp strength to the range [0, 100]
    strength = std::max<int>(0, std::min<int>(strength, 100));

    // Set progress bar and text
    pProgress->SetPos(strength);
    if (strength < 30)
    {
        pStrengthText->SetWindowTextW(_T("Very Weak"));
    }
    else if (strength < 60)
    {
        pStrengthText->SetWindowTextW(_T("Weak"));
    }
    else if (strength < 90)
    {
        pStrengthText->SetWindowTextW(_T("Moderate"));
    }
    else
    {
        pStrengthText->SetWindowTextW(_T("Strong"));
    }
}


void CChangeUserPasswordDlg::OnEnChangeEditEnterpassword()
{
    auto pPassword = (CStatic*)GetDlgItem(IDC_EDIT_ENTERPASSWORD);
    CString password;
    pPassword->GetWindowTextW(password);
    CheckPasswordStrength(password);
}


void CChangeUserPasswordDlg::OnBnClickedCancel()
{
    this->EndDialog(IDCANCEL);
}


CString CChangeUserPasswordDlg::GetCurrentUsername()
{
    auto resultSet = db->ExecuteQuery("SELECT CURRENT_USER() AS CurrentUser");
    CString username;
    while (resultSet->next()) {
        username = SQLStringToCString(resultSet->getString("CurrentUser"));
    }
    if (username.GetLength() >= 2) {
        username.Delete(username.GetLength() - 2, 2);
    }
    delete resultSet;
    return username;
}

bool CChangeUserPasswordDlg::GetEnteredPasswords(CString& new_password, CString& retyped)
{
    auto pPassword = (CEdit*)GetDlgItem(IDC_EDIT_ENTERPASSWORD);
    auto PEditRetype = (CEdit*)GetDlgItem(IDC_EDIT_RETYPE);
    if (pPassword && PEditRetype) {
        pPassword->GetWindowTextW(new_password);
        PEditRetype->GetWindowTextW(retyped);
        return true;
    }
    else {
        AfxMessageBox(_T("Error accessing password fields."));
        return false;
    }
}

bool CChangeUserPasswordDlg::IsNoPasswordSelected()
{
    auto pRadioPassword = (CButton*)GetDlgItem(IDC_RADIO_NOPASSWORD);
    if (pRadioPassword)
    {
        return pRadioPassword->GetCheck() == BST_CHECKED;
    }
}

bool CChangeUserPasswordDlg::UpdatePassword(const CString& username, const CString& new_password)
{
    CString sqlQuery;
    sqlQuery.Format(_T("SET PASSWORD FOR '%s' = '%s'"), username, new_password);
    return db->ExecuteNonQuery(CStringToSQLString(sqlQuery));
}



void CChangeUserPasswordDlg::OnBnClickedRadioNopassword()
{
    auto pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_STRENGTH);
    auto pStrengthText = (CStatic*)GetDlgItem(IDC_STATIC_STRENGTH);
    auto pEditPassword = (CStatic*)GetDlgItem(IDC_EDIT_ENTERPASSWORD);
    auto PEditRetype = (CStatic*)GetDlgItem(IDC_EDIT_RETYPE);
    auto GenPassword = (CStatic*)GetDlgItem(IDC_EDIT_GENPASSWORD);
    auto btnGenerate = (CStatic*)GetDlgItem(IDC_BTN_GENERATE);

    pEditPassword->SetWindowTextW(L"");
    PEditRetype->SetWindowTextW(L"");
    GenPassword->SetWindowTextW(L"");

    pProgress->EnableWindow(FALSE);
    pStrengthText->EnableWindow(FALSE);
    pEditPassword->EnableWindow(FALSE);
    PEditRetype->EnableWindow(FALSE);
    GenPassword->EnableWindow(FALSE);
    btnGenerate->EnableWindow(FALSE);
}


void CChangeUserPasswordDlg::OnBnClickedRadioPassword()
{
    // Get pointers to the controls
    auto pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_STRENGTH);
    auto pStrengthText = (CStatic*)GetDlgItem(IDC_STATIC_STRENGTH);
    auto pEditPassword = (CStatic*)GetDlgItem(IDC_EDIT_ENTERPASSWORD);
    auto PEditRetype = (CStatic*)GetDlgItem(IDC_EDIT_RETYPE);
    auto GenPassword = (CStatic*)GetDlgItem(IDC_EDIT_GENPASSWORD);
    auto btnGenerate = (CStatic*)GetDlgItem(IDC_BTN_GENERATE);

    // Disable all controls
    pProgress->EnableWindow(TRUE);
    pStrengthText->EnableWindow(TRUE);
    pEditPassword->EnableWindow(TRUE);
    PEditRetype->EnableWindow(TRUE);
    GenPassword->EnableWindow(TRUE);
    btnGenerate->EnableWindow(TRUE);
}


void CChangeUserPasswordDlg::OnBnClickedChangepwd()
{
    // Get current user
    CString username = GetCurrentUsername();

    // Get entered passwords
    CString new_password, retyped;
    if (!GetEnteredPasswords(new_password, retyped)) {
        return; // Error message already displayed
    }

    // Check if password should be empty
    if (IsNoPasswordSelected()) {
        new_password.Empty();
    }

    // Check if the new password is empty
    if (new_password.IsEmpty() && !IsNoPasswordSelected()) {
        AfxMessageBox(_T("Password edit box cannot be blank."));
        return;
    }

    // Check if passwords match
    if (new_password == retyped) {
        // Update the password in the database
        if (UpdatePassword(username, new_password)) {
            AfxMessageBox(_T("Password changed."));
        }
        else {
            AfxMessageBox(_T("Failed to update password. Check your input and try again."));
        }
    }
    else {
        AfxMessageBox(_T("Passwords are not equal."));
    }
}