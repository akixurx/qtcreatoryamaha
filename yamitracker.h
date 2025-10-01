#ifndef YAMITRACKER_H
#define YAMITRACKER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Yamitracker; }
QT_END_NAMESPACE

class Yamitracker : public QMainWindow
{
    Q_OBJECT

public:
    Yamitracker(QWidget *parent = nullptr);
    ~Yamitracker();

private:
    Ui::Yamitracker *ui;
};
#endif // YAMITRACKER_H
