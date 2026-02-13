#pragma once
#include <QWidget>

class ViewportController;

/**
 * @brief The UI Layer.
 * A dumb container that forwards Qt Events to the Controller.
 * Sets critical Qt Attributes to allow OCCT to render into the window.
 */
class OcctWidget : public QWidget
{
	Q_OBJECT
public:
	explicit OcctWidget(QWidget* parent = nullptr);

	// Dependency Injection for the Controller
	void setController(ViewportController* controller) { m_controller = controller; }

protected:
	// Important: Tell Qt not to paint over our OCCT content
	QPaintEngine* paintEngine() const override { return nullptr; }

	// Event Overrides
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