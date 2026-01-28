#include <QWidget>

// TODO: Create a window that OcctViewport can live inside
class OcctWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OcctWidget(QWidget *parent = nullptr);

protected:
    // Important: Tell Qt not to paint over our OCCT content
    QPaintEngine *paintEngine() const override { return nullptr; }
    void paintEvent(QPaintEvent *e) override {}

    // Captured events are forwarded to the Core via signals or callbacks
    void resizeEvent(QResizeEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
};