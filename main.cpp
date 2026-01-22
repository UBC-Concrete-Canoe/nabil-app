#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

int main(int argc, char *argv[])
{
    // Initialize the application
    QApplication app(argc, argv);

    // Create a main widget (the window)
    QWidget window;
    window.setWindowTitle("Qt VM Test");
    window.resize(400, 300);

    // Create a layout
    QVBoxLayout *layout = new QVBoxLayout(&window);

    // Add a label
    QLabel *label = new QLabel("If you see this, basic Qt windowing works!", &window);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    // Add a button that closes the window
    QPushButton *button = new QPushButton("Close Test", &window);
    layout->addWidget(button);

    // Connect the button click to the application quit slot
    QObject::connect(button, &QPushButton::clicked, &app, &QApplication::quit);

    // Display the window
    window.show();

    // Run the event loop
    return app.exec();
}