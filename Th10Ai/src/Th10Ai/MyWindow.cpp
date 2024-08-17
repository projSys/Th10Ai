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
		m_buffer(m_size),
		m_statusUpdated(false)
	{
		SetClientSize(m_size);
		SetMinClientSize(m_size);
		SetBackgroundColour(*wxWHITE);
		SetBackgroundStyle(wxBG_STYLE_PAINT);

		m_writableStatus = std::make_unique<Status>();
		m_swappableStatus = std::make_unique<Status>();
		m_readableStatus = std::make_unique<Status>();
	}

	MyWindow::~MyWindow()
	{
	}

	void MyWindow::onPaint(wxPaintEvent& event)
	{
		wxBufferedPaintDC dc(this, m_buffer);
		dc.SetBrush(*wxWHITE_BRUSH);
		dc.Clear();

		{
			std::lock_guard<std::mutex> lock(m_statusMutex);
			if (m_statusUpdated)
			{
				m_readableStatus.swap(m_swappableStatus);
				m_statusUpdated = false;
			}
		}

		dc.SetPen(*wxGREEN_PEN);
		dc.SetBrush(*wxGREEN_BRUSH);
		const Player& player = m_readableStatus->getPlayer();
		dc.DrawRectangle(
			std::round(player.pos.x - player.size.x / 2.0 + 192.0),
			std::round(player.pos.y - player.size.y / 2.0),
			std::round(player.size.x),
			std::round(player.size.y));

		dc.SetPen(*wxBLUE_PEN);
		dc.SetBrush(wxNullBrush);
		const std::vector<Item>& items = m_readableStatus->getItems();
		for (const Item& item : items)
		{
			dc.DrawRectangle(
				std::round(item.pos.x - item.size.x / 2.0 + 192.0),
				std::round(item.pos.y - item.size.y / 2.0),
				std::round(item.size.x),
				std::round(item.size.y));
		}

		dc.SetPen(*wxRED_PEN);
		dc.SetBrush(*wxRED_BRUSH);
		const std::vector<Enemy>& enemies = m_readableStatus->getEnemies();
		for (const Enemy& enemy : enemies)
		{
			dc.DrawRectangle(
				std::round(enemy.pos.x - enemy.size.x / 2.0 + 192.0),
				std::round(enemy.pos.y - enemy.size.y / 2.0),
				std::round(enemy.size.x),
				std::round(enemy.size.y));
		}

		dc.SetPen(*wxRED_PEN);
		dc.SetBrush(wxNullBrush);
		const std::vector<Bullet>& bullets = m_readableStatus->getBullets();
		for (const Bullet& bullet : bullets)
		{
			dc.DrawRectangle(
				std::round(bullet.pos.x - bullet.size.x / 2.0 + 192.0),
				std::round(bullet.pos.y - bullet.size.y / 2.0),
				std::round(bullet.size.x),
				std::round(bullet.size.y));
		}

		dc.SetPen(*wxRED_PEN);
		dc.SetBrush(wxNullBrush);
		const std::vector<Laser>& lasers = m_readableStatus->getLasers();
		for (const Laser& laser : lasers)
		{
			wxPoint p1(std::round(laser.topLeft.x + 192.0), std::round(laser.topLeft.y));
			wxPoint p2(std::round(laser.topRight.x + 192.0), std::round(laser.topRight.y));
			wxPoint p3(std::round(laser.bottomRight.x + 192.0), std::round(laser.bottomRight.y));
			wxPoint p4(std::round(laser.bottomLeft.x + 192.0), std::round(laser.bottomLeft.y));
			dc.DrawLine(p1, p2);
			dc.DrawLine(p2, p3);
			dc.DrawLine(p3, p4);
			dc.DrawLine(p4, p1);
		}
	}

	// 空函数，防止闪烁
	void MyWindow::onEraseBackground(wxEraseEvent& event)
	{
	}

	void MyWindow::onStatusUpdate(const Status& status)
	{
		m_writableStatus->copy(status);
		{
			std::lock_guard<std::mutex> lock(m_statusMutex);
			m_writableStatus.swap(m_swappableStatus);
			m_statusUpdated = true;
		}
		Refresh(false);
		//Update();
	}
}
