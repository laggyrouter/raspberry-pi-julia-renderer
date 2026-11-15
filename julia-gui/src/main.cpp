#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("Julia Set Renderer");
    window.resize(800, 480);

    QWidget *central = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(central);

    QLabel *title = new QLabel("JULIA SET RENDERER");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "color: #00e5ff;"
        "font-size: 28px;"
        "font-weight: bold;"
    );

    QLabel *subtitle = new QLabel("Parallel Fractal Visualization");
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet(
        "color: #b388ff;"
        "font-size: 16px;"
    );

    layout->addStretch();
    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addStretch();

    central->setStyleSheet("background-color: #10121c;");

    window.setCentralWidget(central);
    window.show();

    return app.exec();
}
