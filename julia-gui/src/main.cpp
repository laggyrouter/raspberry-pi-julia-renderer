#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QPixmap>

#include "julia_renderer.h"


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
        "font-size: 24px;"
        "font-weight: bold;"
    );

    QLabel *imageLabel = new QLabel();
    imageLabel->setAlignment(Qt::AlignCenter);

    JuliaParameters params;
    params.width = 800;
    params.height = 400;

    QImage fractal = JuliaRenderer::render(params);


imageLabel->setPixmap(QPixmap::fromImage(fractal));

    layout->addWidget(title);
    layout->addWidget(imageLabel, 1);

    central->setStyleSheet("background-color: #10121c;");

    window.setCentralWidget(central);
    window.show();

    return app.exec();
}
