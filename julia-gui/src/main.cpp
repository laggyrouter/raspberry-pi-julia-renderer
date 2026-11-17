#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QSlider>
#include <QAbstractSlider>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <functional>

#include "julia_renderer.h"

class ClickableLabel : public QLabel {
public:
    using QLabel::QLabel;

    std::function<void(QPoint)> onClick;

protected:
    void mousePressEvent(QMouseEvent *event)
override {
    if (onClick) {
        onClick(event->pos());
    }
}
};
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

    ClickableLabel *imageLabel = new ClickableLabel();
    imageLabel->setMouseTracking(true);
    imageLabel->setAlignment(Qt::AlignCenter);

    JuliaParameters params;
    params.width = 800;
    params.height = 340;

    QSlider *zoomSlider = new QSlider(Qt::Horizontal);
    zoomSlider->setRange(10, 100);
    zoomSlider->setValue(10);

    QPushButton *zoomInButton = new QPushButton("+");
    QPushButton *zoomOutButton = new QPushButton("-");
    QPushButton *resetButton = new QPushButton("Reset");

    QLabel *zoomLabel = new QLabel("Zoom: 1.0x");

    //morph controls
    QSlider *realSlider = new QSlider(Qt::Horizontal);
    realSlider->setRange(-2000, 2000);
    realSlider->setValue(-800);

    QSlider *imagSlider = new QSlider(Qt::Horizontal);
    imagSlider->setRange(-2000, 2000);
    imagSlider->setValue(156);

    QLabel *realLabel = new QLabel("C Real: -0.800");
    QLabel *imagLabel = new QLabel("C Imag: 0.156");

    realLabel->setStyleSheet(
    "color: #00e5ff; font-weight: bold;");
    imagLabel->setStyleSheet(
    "color: #00e5ff; font-weight: bold;");

    QHBoxLayout *controls = new QHBoxLayout();

    QHBoxLayout *realControls = new QHBoxLayout();
    realControls->addWidget(realLabel);
    realControls->addWidget(realSlider);

    QHBoxLayout *imagControls = new QHBoxLayout();
    imagControls->addWidget(imagLabel);
    imagControls->addWidget(imagSlider);

    controls->addWidget(zoomOutButton);
    controls->addWidget(zoomSlider);
    controls->addWidget(zoomInButton);
    controls->addWidget(resetButton);
    controls->addWidget(zoomLabel);

    auto updateFractal = [&]() {
        params.zoom = zoomSlider->value() / 10.0;

        QImage fractal = JuliaRenderer::render(params);

    imageLabel->setPixmap(QPixmap::fromImage(fractal));

        zoomLabel->setText(QString("Zoom: %1x").arg(params.zoom, 0, 'f', 1));
    };

    imageLabel->onClick = [&] (QPoint pos) {

        params.zoom = zoomSlider->value() / 10.0;

        double aspectRatio = static_cast<double>(params.width) / params.height;

        double viewHeight = 2.7 / params.zoom;
        double viewWidth = viewHeight * aspectRatio;

        params.centerX += (static_cast<double>(pos.x()) / params.width - 0.5)
        *viewWidth;

        params.centerY += (0.5 - static_cast<double>(pos.y()) / params.height)
        *viewHeight;

        updateFractal();
    };


    QObject::connect(
        zoomSlider,
        &QSlider::valueChanged,
        &window,
        [&] (int) {
            updateFractal();
        }
    );

    QObject::connect(realSlider,
    &QSlider::valueChanged,
    &window, [&] (int value) {
        params.cReal = value / 1000.0;
        realLabel->setText(
            QString("C Real: %1").arg(params.cReal, 0, 
    'f', 3));
    }
    );

    QObject::connect(imagSlider,
    &QSlider::valueChanged,
    &window, [&] (int value) {
        params.cImag = value / 1000.0;
        imagLabel->setText(
            QString("C Imag: %1").arg(params.cImag, 0,
    'f', 3));
    }
    );

    QObject::connect(realSlider,
    &QAbstractSlider::sliderReleased,
    &window, [&] () {
        updateFractal();
    }
    );

    QObject::connect(imagSlider,
    &QAbstractSlider::sliderReleased,
    &window, [&] () {
        updateFractal();
    }
    );

    QObject::connect(
        zoomInButton,
        &QPushButton::clicked,
        zoomSlider,
        [zoomSlider]() {
            zoomSlider->setValue(zoomSlider->value() + 5);
        }
    );

    QObject::connect(
        zoomOutButton,
        &QPushButton::clicked,
        zoomSlider,
        [zoomSlider]() {
            zoomSlider->setValue(zoomSlider->value() - 5);
        }
    );

    QObject::connect(
        resetButton,
        &QPushButton::clicked,
        zoomSlider,
        [zoomSlider]() {
            zoomSlider->setValue(10);
        }
    );


    layout->addWidget(title);
    layout->addWidget(imageLabel);
    layout->addLayout(controls);
    layout->addLayout(realControls);
    layout->addLayout(imagControls);

    updateFractal();

    central->setStyleSheet("background-color: #10121c;");

    window.setCentralWidget(central);
    window.show();

    return app.exec();
}
