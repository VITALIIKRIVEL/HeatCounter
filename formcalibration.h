#ifndef FORMCALIBRATION_H
#define FORMCALIBRATION_H

#include <QWidget>
#include <QMap>

namespace Ui {
class FormCalibration;
}

class FormCalibration : public QWidget
{
    Q_OBJECT

public:
    explicit FormCalibration(QWidget *parent = 0);
    ~FormCalibration();

signals:
    startCalibration(QMap<QString, QByteArray>, QMap<QString, float>);
    signalStartRealClockCalibration(quint16);

//    signalCalibrationFloatMap(QMap<QString, float>);

private slots:
    void on_toolButton_selectConfigFile_clicked();

    void on_toolButton_startCalibration_clicked();

    void on_lineEdit_R_Real1_textChanged(const QString &arg1);

    void on_lineEdit_R_Real2_textChanged(const QString &arg1);

    void on_lineEdit_Temperature1_textChanged(const QString &arg1);

    void on_lineEdit_Temperature2_textChanged(const QString &arg1);

    void on_comboBox_RTD_Type_currentIndexChanged(const QString &arg1);

    void on_toolButton_realClockCalibration_clicked();

    void on_lineEdit_periodBetweenPulses_textChanged(const QString &arg1);

private:
    Ui::FormCalibration *ui;

    QMap<QString, QByteArray> calibrationMap;
    QMap<QString, float> calibrationFloatMap;

    QByteArray R_Real1ByteArray;
    QByteArray R_Real2ByteArray;
    QByteArray temperature1ByteArray;
    QByteArray temperature2ByteArray;
//    float Temperature1_type0;
//    float Temperature2_type0;
//    float Temperature1_type1;
//    float Temperature2_type1;
//    float Temperature1_type2;
//    float Temperature2_type2;
//    float Temperature1_type3;
//    float Temperature2_type3;
    float R_Ref1Float;
    float R_Ref2Float;
    float Acceptable_R_Ref1Float;
    float Acceptable_R_Ref2Float;
    float Acceptable_TMeasDirFloat;
    float Acceptable_TMeasRevFloat;
    float Error_R_Reff1Float;
    float Error_R_Reff2Float;
    float Error_TMeasDirFloat;
    float Error_TMeasRevFloat;
    int RTD_TypeInt;

    //калибровка генератора часов
    float periodBetweenPulses;
};

#endif // FORMCALIBRATION_H
