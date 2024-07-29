#pragma once

#include "pattern_generator.hpp"

#include <QQuickImageProvider>
#include <QTimer>
#include <cstdint>
#include <memory>

class ImageProvider : public QQuickImageProvider
{
  Q_OBJECT

public:
  ImageProvider(QObject* parent = nullptr);
  QQmlImageProviderBase::ImageType imageType() const override;
  QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override;

public slots:
  void startStopGeneration();
  void resetImage();

private slots:
  void updateImage();

signals:
  void imageReady();

private:
  QTimer* _timer;
  std::unique_ptr<PatternGenerator> _patternGenerator;

  void instantiateGenerator(const QSize& size);
};
