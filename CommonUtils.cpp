#include "CommonUtils.h"

CommonUtils::CommonUtils()
{
}

CommonUtils::~CommonUtils()
{
}

//获取圆头像(原图,空图,图大小)
QPixmap CommonUtils::getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize)
{
	if (maskSize == QSize(0, 0)) {
		maskSize = mask.size();
	}
	else {
		//缩放(圆头像,尽可能大的矩形(保持纵横比),转换模式::平滑转换)
		mask = mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	//保存转换后的图像(图大小,什么形式获取图像::格式ARGB32自左乘)
	QImage resultImage(maskSize, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&resultImage);//画家.设置作品模式(画家::原始) //设置图片叠加模式
	painter.setCompositionMode(QPainter::CompositionMode_Source);//将原图像进行复制的操作
	painter.fillRect(resultImage.rect(), Qt::transparent);//画家.填充矩形(矩形大小,透明)
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);//覆盖
	painter.drawPixmap(0, 0, mask);//mask进行叠加
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);//在...里
	painter.drawPixmap(0, 0, src.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));//再将src叠加进去
	painter.end();

	return QPixmap::fromImage(resultImage);
}

//加载样式表
void CommonUtils::loadStyleSheet(QWidget* widget, const QString& sheetName)
{
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen()) {
		widget->setStyleSheet("");//设置风格覆盖
		QString qsstyleSheet = QLatin1String(file.readAll());//读取所有
		widget->setStyleSheet(qsstyleSheet);
		file.close();
	}
}

//设置默认皮肤颜色
void CommonUtils::setDefaultSkinColor(const QColor& color)
{
	//将配置文件的颜色进行重写
	//路径
	const QString&& path = QApplication::applicationDirPath() + "/" + QString("tradeprintinfo.ini");

	QSettings settings(path, QSettings::IniFormat);//配置文件进行读写操作
	//[DefaultSkin]是节,这是定位
	settings.setValue("DefaultSkin/red", color.red());//设置将key设置为value的值。如果键已经存在，则覆盖前面的值。
	settings.setValue("DefaultSkin/green", color.green());
	settings.setValue("DefaultSkin/blue", color.blue());
	//settings.clear();//这会清空整个配置文件
}

//获取默认皮肤颜色
QColor CommonUtils::getDefaultSkinColor()
{
	QColor color;
	//路径
	const QString&& path = QApplication::applicationDirPath() + "/" + QString("tradeprintinfo.ini");
	if (!QFile::exists(path)) //文件不存在
		setDefaultSkinColor(QColor(192, 253, 123));//默认颜色

	QSettings settings(path, QSettings::IniFormat);//配置文件进行读写操作

	color.setRed(settings.value("DefaultSkin/red").toInt());//将此颜色的红色组件设置为红色。整数组件在0-255范围内指定。
	color.setGreen(settings.value("DefaultSkin/green").toInt());//返回设置键的值。如果该设置不存在，则返回defaultValue。
	color.setBlue(settings.value("DefaultSkin/blue").toInt());

	return color;
}
