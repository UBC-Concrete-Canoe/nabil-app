#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QFrame;
class QShortcut;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
} // namespace Ui
QT_END_NAMESPACE

/**
 * @brief Top-level Qt window that hosts the viewport layout and controls.
 */
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/**
	 * @brief Construct the main application window.
	 * @param parent Optional parent widget.
	 */
	explicit MainWindow(QWidget* parent = nullptr);
	//! Destroy owned UI resources.
	~MainWindow();

private:
	/**
	 * @brief Maximize one viewport frame, or restore all four if it is already maximized.
	 * @param target The viewport frame to maximize.
	 */
	void toggleMaximize(QFrame* target);

	//! Designer-generated UI object.
	Ui::MainWindow* ui;
	//! The four viewport frames: perspective, plan, profile, bodyplan.
	QList<QFrame*> m_viewportFrames;
	//! The currently maximized viewport frame, or nullptr when all four are shown.
	QFrame* m_maximized = nullptr;
	//! Esc restores the four viewports; only enabled while one is maximized.
	QShortcut* m_restoreShortcut = nullptr;
};
#endif // MAINWINDOW_H
