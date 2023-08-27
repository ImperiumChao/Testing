#ifndef TESTSETTINGS_H
#define TESTSETTINGS_H

#include <QWidget>

namespace Ui {
class TestSettings;
}

class TestSettings : public QWidget
{
    Q_OBJECT

public:
    explicit TestSettings(QWidget *parent, QStringList sub, QStringList user);
    ~TestSettings();
    QStringList subject;
    void fillData(QStringList test);

private slots:

    void on_pushCreate_clicked();

    void on_pushCancel_clicked();


    void on_pushAddOwner_clicked();

    void on_pushDelOwner_clicked();


    void on_toolImport_clicked();

    void on_toolExport_clicked();

private:
    void n_test();
    Ui::TestSettings *ui;
    QStringList u;
    bool neW;
    QStringList t;
    QList<QStringList> owners, teachers;
signals:
    void addedTest();
    void editedTest();
};

#endif // TESTSETTINGS_H
