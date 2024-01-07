#pragma once

#include <afxwin.h>
#include <vector>

class Drawer
{

	CRect frame;
	CWnd* wnd;
	CDC* dc;
	CDC memDC;
	CBitmap bmp;
	bool init;

public:
	void Create(HWND hWnd)
	{
		wnd = CWnd::FromHandle(hWnd);
		wnd->GetClientRect(frame);
		dc = wnd->GetDC();

		memDC.CreateCompatibleDC(dc);
		bmp.CreateCompatibleBitmap(dc, frame.Width(), frame.Height());
		memDC.SelectObject(&bmp);
		init = true;
	}

	void Draw(
		double data_y_max, double data_y_min,
		double data_x_max, double data_x_min,
		std::vector<double>& y_first_graph, 
		std::vector<double>& x_first_graph = std::vector<double>())
	{
		if (!init) return;

		CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
		CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen data_pen(PS_SOLID, 2, RGB(216, 120, 29)); //pen 1 color 
		CPen data_pen2(PS_SOLID, 2, RGB(0, 255, 0));
		CPen data_pen3(PS_SOLID, 2, RGB(0, 0, 255));
		CFont font;
		font.CreateFontW(18, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				
			FALSE,				
			FALSE,				
			DEFAULT_CHARSET,	
			OUT_OUTLINE_PRECIS,	
			CLIP_DEFAULT_PRECIS,
			100,				
			VARIABLE_PITCH,
			TEXT("Times New Roman")
		);

		int padding = 20;
		int left_keys_padding = 20;
		int bottom_keys_padding = 10;

		int actual_width = frame.Width() - 2 * padding - left_keys_padding;
		int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		int actual_top = padding;
		int actual_bottom = actual_top + actual_height;
		int actual_left = padding + left_keys_padding;
		int actual_right = actual_left + actual_width;

		memDC.FillSolidRect(frame, RGB(255, 255, 255)); //rect color

		unsigned int grid_size = 10;
		memDC.SelectObject(&subgrid_pen);
		for (double i = 0.5; i < grid_size; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}
		memDC.SelectObject(&grid_pen);
		for (double i = 0.0; i < grid_size + 1; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		if (y_first_graph.empty()) return;

		if (x_first_graph.size() != y_first_graph.size())
		{
			x_first_graph.resize(y_first_graph.size());
			for (int i = 0; i < x_first_graph.size(); i++)
			{
				x_first_graph[i] = i;
			}
		}
		std::vector<double> y = convert_range(y_first_graph, actual_top, actual_bottom, data_y_max, data_y_min);
		std::vector<double> x = convert_range(x_first_graph, actual_right, actual_left, data_x_max, data_x_min);
		memDC.MoveTo(x[0], y[0]);
		for (int i = 0; i < grid_size / 2 + 1; i++)
		{
			CString str;
			str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
			memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

			str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
			memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
		}
		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(&data_pen);
		memDC.MoveTo(x[0], y[0]);
		for (unsigned int i = 0; i < y.size(); i++)
		{
			memDC.LineTo(x[i], y[i]);
			dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
		}
		memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0, 0, 0));
		for (int i = 0; i < grid_size / 2 + 1; i++)
		{
			CString str;
			str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
			memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

			str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
			memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
		}
		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}

	std::vector<double> convert_range(std::vector <double>& data, double outmax, 
									double outmin,
									double inmax, double inmin)
	{
		std::vector<double> output = data;
		double k = (outmax - outmin) / (inmax - inmin);
		for (auto& item : output)
		{
			item = (item - inmin) * k + outmin;
		}
		return output;
	}
};