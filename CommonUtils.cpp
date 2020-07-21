#include "CommonUtils.h"

CommonUtils::CommonUtils()
{
}

CommonUtils::~CommonUtils()
{
}

//��ȡԲͷ��(ԭͼ,��ͼ,ͼ��С)
QPixmap CommonUtils::getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize)
{
	if (maskSize == QSize(0, 0)) {
		maskSize = mask.size();
	}
	else {
		//����(Բͷ��,�����ܴ�ľ���(�����ݺ��),ת��ģʽ::ƽ��ת��)
		mask = mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	//����ת�����ͼ��(ͼ��С,ʲô��ʽ��ȡͼ��::��ʽARGB32�����)
	QImage resultImage(maskSize, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&resultImage);//����.������Ʒģʽ(����::ԭʼ) //����ͼƬ����ģʽ
	painter.setCompositionMode(QPainter::CompositionMode_Source);//��ԭͼ����и��ƵĲ���
	painter.fillRect(resultImage.rect(), Qt::transparent);//����.������(���δ�С,͸��)
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);//����
	painter.drawPixmap(0, 0, mask);//mask���е���
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);//��...��
	painter.drawPixmap(0, 0, src.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));//�ٽ�src���ӽ�ȥ
	painter.end();

	return QPixmap::fromImage(resultImage);
}

//������ʽ��
void CommonUtils::loadStyleSheet(QWidget* widget, const QString& sheetName)
{
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen()) {
		widget->setStyleSheet("");//���÷�񸲸�
		QString qsstyleSheet = QLatin1String(file.readAll());//��ȡ����
		widget->setStyleSheet(qsstyleSheet);
		file.close();
	}
}

//����Ĭ��Ƥ����ɫ
void CommonUtils::setDefaultSkinColor(const QColor& color)
{
	//�������ļ�����ɫ������д
	//·��
	const QString&& path = QApplication::applicationDirPath() + "/" + QString("tradeprintinfo.ini");

	QSettings settings(path, QSettings::IniFormat);//�����ļ����ж�д����
	//[DefaultSkin]�ǽ�,���Ƕ�λ
	settings.setValue("DefaultSkin/red", color.red());//���ý�key����Ϊvalue��ֵ��������Ѿ����ڣ��򸲸�ǰ���ֵ��
	settings.setValue("DefaultSkin/green", color.green());
	settings.setValue("DefaultSkin/blue", color.blue());
	//settings.clear();//���������������ļ�
}

//��ȡĬ��Ƥ����ɫ
QColor CommonUtils::getDefaultSkinColor()
{
	QColor color;
	//·��
	const QString&& path = QApplication::applicationDirPath() + "/" + QString("tradeprintinfo.ini");
	if (!QFile::exists(path)) //�ļ�������
		setDefaultSkinColor(QColor(192, 253, 123));//Ĭ����ɫ

	QSettings settings(path, QSettings::IniFormat);//�����ļ����ж�д����

	color.setRed(settings.value("DefaultSkin/red").toInt());//������ɫ�ĺ�ɫ�������Ϊ��ɫ�����������0-255��Χ��ָ����
	color.setGreen(settings.value("DefaultSkin/green").toInt());//�������ü���ֵ����������ò����ڣ��򷵻�defaultValue��
	color.setBlue(settings.value("DefaultSkin/blue").toInt());

	return color;
}
