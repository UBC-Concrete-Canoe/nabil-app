#pragma once
#include <QWidget>

class ViewportController;

/**
 * @brief UI widget that hosts OCCT rendering.
 *
 * This widget serves as a bridge between Qt and OpenCascade Technology (OCCT). It:
 * - Disables Qt's paint engine to allow OCCT direct hardware access
 * - Forwards input events to the ViewportController
 * - Manages critical Qt attributes for OCCT rendering
 *
 * The widget requires a ViewportController to be injected via setController() to
 * properly handle user interactions.
 */
class OcctWidget : public QWidget
{
	Q_OBJECT
public:
	//! Constructor.
	explicit OcctWidget(QWidget* parent = nullptr);

	/**
	 * @brief Inject the controller that interprets user input.
	 * @param controller Ownership remains with caller
	 */
	void setController(ViewportController* controller) { m_controller = controller; }

protected:
	QPaintEngine* paintEngine() const override { return nullptr; }

	void resizeEvent(QResizeEvent* e) override;
	void paintEvent(QPaintEvent* e) override;

	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;
	void wheelEvent(QWheelEvent* e) override;
	void keyPressEvent(QKeyEvent* e) override;

private:
	ViewportController* m_controller = nullptr;
};