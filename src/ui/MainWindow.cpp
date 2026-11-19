#include "MainWindow.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QShortcut>
#include <QToolButton>
#include <QVBoxLayout>
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_viewportFrames = {ui->frame_4, ui->frame_2, ui->frame, ui->frame_3};

	// Add a maximize/restore button to the right of each viewport's label. Built here
	// rather than in MainWindow.ui because it is identical for all four viewports.
	for (QFrame* frame : m_viewportFrames)
	{
		// The central widget is right-to-left (to place the perspective view top left);
		// keep the inside of each frame left-to-right so the button sits on the right.
		frame->setLayoutDirection(Qt::LeftToRight);

		auto* button = new QToolButton(frame);
		button->setAutoRaise(true);
		button->setIcon(QIcon(":/maximize.png"));
		button->setToolTip(tr("Maximize"));
		connect(button, &QToolButton::clicked, this, [this, frame] { toggleMaximize(frame); });

		QLabel* label = frame->findChild<QLabel*>();
		auto* frameLayout = qobject_cast<QVBoxLayout*>(frame->layout());
		frameLayout->removeWidget(label);

		auto* header = new QHBoxLayout;
		header->addWidget(label);
		header->addStretch();
		header->addWidget(button);
		frameLayout->insertLayout(0, header);
	}

	// Disabled until a viewport is maximized, so Esc still reaches the viewports otherwise.
	m_restoreShortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
	m_restoreShortcut->setEnabled(false);
	connect(m_restoreShortcut, &QShortcut::activated, this, [this] { toggleMaximize(m_maximized); });
}

MainWindow::~MainWindow()
{
	delete ui;
}

void
MainWindow::toggleMaximize(QFrame* target)
{
	// Clicking the frame that is already maximized restores the 2x2 grid.
	const bool restoring = (m_maximized == target);

	// Hide rather than reparent: each OcctWidget is a native window that OCCT is
	// bound to, and hiding keeps that window handle alive.
	for (QFrame* frame : m_viewportFrames)
	{
		frame->setVisible(restoring || frame == target);
	}

	// QGridLayout keeps space for a row or column with a stretch factor even when
	// everything in it is hidden, so zero the stretch of the target's neighbours.
	// The frames sit in rows 1 and 3 and columns 0 and 1 (see MainWindow.ui).
	QGridLayout* grid = ui->gridLayout_2;
	int row = 0, col = 0, rowSpan = 0, colSpan = 0;
	grid->getItemPosition(grid->indexOf(target), &row, &col, &rowSpan, &colSpan);
	for (int r : {1, 3})
	{
		grid->setRowStretch(r, (restoring || r == row) ? 1 : 0);
	}
	for (int c : {0, 1})
	{
		grid->setColumnStretch(c, (restoring || c == col) ? 1 : 0);
	}

	m_maximized = restoring ? nullptr : target;
	m_restoreShortcut->setEnabled(m_maximized != nullptr);

	// The target's button now does the opposite of what it just did.
	QToolButton* button = target->findChild<QToolButton*>();
	button->setIcon(QIcon(restoring ? ":/maximize.png" : ":/restore.png"));
	button->setToolTip(restoring ? tr("Maximize") : tr("Restore"));
}
