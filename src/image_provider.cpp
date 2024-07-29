#include "image_provider.hpp"

ImageProvider::ImageProvider(QObject* parent) : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
  instantiateGenerator(QSize(320, 240));

  _timer = new QTimer(this);
  connect(_timer, &QTimer::timeout, this, &ImageProvider::updateImage);
  _timer->setInterval(1);
  _timer->setSingleShot(false);
}

QQmlImageProviderBase::ImageType ImageProvider::imageType() const
{
  return QQmlImageProviderBase::Image;
}

QImage ImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize)
{
  int width = 320;
  int height = 240;

  if (size) {
    *size = QSize(width, height);
  }

  QImage image(requestedSize.width() > 0 ? requestedSize.width() : width,
    requestedSize.height() > 0 ? requestedSize.height() : height, QImage::Format_Indexed8);

  for (size_t i = 0; i < image.width() * image.height(); i++) {
    image.bits()[i] = static_cast<unsigned char>(_patternGenerator->pattern()[i] * 255.0);
  }

  return image;
}

void ImageProvider::startStopGeneration()
{
  if (_timer->isActive()) {
    _timer->stop();
  } else {
    _timer->start();
  }
}

void ImageProvider::resetImage()
{
  _patternGenerator->randomize();
  emit imageReady();
}

void ImageProvider::updateImage()
{
  if (_patternGenerator) {
    _patternGenerator->generate();
    emit imageReady();
  }
}

void ImageProvider::instantiateGenerator(const QSize& size)
{
  _patternGenerator = std::make_unique<PatternGenerator>(size.width(), size.height());
  _patternGenerator->_scales.push_back(Scale(50, 100, 0.05));
  _patternGenerator->_scales.push_back(Scale(25, 50, 0.04));
  _patternGenerator->_scales.push_back(Scale(12, 25, 0.03));
  _patternGenerator->_scales.push_back(Scale(6, 12, 0.02));
  _patternGenerator->_scales.push_back(Scale(3, 6, 0.01));
}
