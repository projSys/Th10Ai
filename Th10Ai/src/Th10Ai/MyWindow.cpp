#include "Th10Ai/Common.h"
#include "Th10Ai/MyWindow.h"

#include <wx/dcbuffer.h>

namespace th
{
	wxBEGIN_EVENT_TABLE(MyWindow, wxWindow)
		EVT_PAINT(MyWindow::onPaint)
		EVT_ERASE_BACKGROUND(MyWindow::onEraseBackground)
	wxEND_EVENT_TABLE()

	MyWindow::MyWindow(wxWindow* parent) :
		wxWindow(parent, wxID_ANY),
		m_size(384, 448),
		m_buffer(m_size)
	{
		SetClientSize(m_size);
		SetMinClientSize(m_size);
		SetBackgroundColour(*wxWHITE);
		SetBackgroundStyle(wxBG_STYLE_PAINT);

		memset(&m_status, 0, sizeof(StatusData));
	}

	MyWindow::~MyWindow()
	{
	}

	void MyWindow::onPaint(wxPaintEvent& event)
	{
		wxBufferedPaintDC dc(this, m_buffer);
		dc.SetBrush(*wxWHITE_BRUSH);
		dc.Clear();

		dc.SetPen(*wxBLACK_PEN);
		dc.SetBrush(*wxBLACK_BRUSH);
		dc.DrawRectangle(std::round(m_status.player.x - m_status.player.width / 2.0 + 192.0),
			std::round(m_status.player.y - m_status.player.height / 2.0),
			std::round(m_status.player.width),
			std::round(m_status.player.height));

		dc.SetPen(*wxBLUE_PEN);
		dc.SetBrush(wxNullBrush);
		for (uint_t i = 0; i < m_status.itemCount; ++i)
		{
			dc.DrawRectangle(std::round(m_status.items[i].x - m_status.items[i].width / 2.0 + 192.0),
				std::round(m_status.items[i].y - m_status.items[i].height / 2.0),
				std::round(m_status.items[i].width),
				std::round(m_status.items[i].height));
		}

		dc.SetPen(*wxRED_PEN);
		dc.SetBrush(*wxRED_BRUSH);
		for (uint_t i = 0; i < m_status.enemyCount; ++i)
		{
			dc.DrawRectangle(std::round(m_status.enemies[i].x - m_status.enemies[i].width / 2.0 + 192.0),
				std::round(m_status.enemies[i].y - m_status.enemies[i].height / 2.0),
				std::round(m_status.enemies[i].width),
				std::round(m_status.enemies[i].height));
		}

		dc.SetPen(*wxRED_PEN);
		dc.SetBrush(wxNullBrush);
		for (uint_t i = 0; i < m_status.bulletCount; ++i)
		{
			dc.DrawRectangle(std::round(m_status.bullets[i].x - m_status.bullets[i].width / 2.0 + 192.0),
				std::round(m_status.bullets[i].y - m_status.bullets[i].height / 2.0),
				std::round(m_status.bullets[i].width),
				std::round(m_status.bullets[i].height));
		}

		for (uint_t i = 0; i < m_status.laserCount; ++i)
		{

		}
	}

	void MyWindow::onEraseBackground(wxEraseEvent& event)
	{
		// 空函数，只为了防止闪烁
	}

	void MyWindow::update(const StatusData& status)
	{
		memcpy(&m_status, &status, sizeof(StatusData));
		Refresh();
		//Update();
	}
}
