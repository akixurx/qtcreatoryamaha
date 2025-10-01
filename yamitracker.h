#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QGridLayout>
#include <QWidget>
#include <QGroupBox>
#include <QMap>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNotePressed(int note);
    void onNoteReleased(int note);
    void onVolumeChanged(int value);
    void onInstrumentChanged(int index);
    void onOctaveChanged(int value);

private:
    void setupUI();
    void createPianoKeys();
    void createControlPanel();
    void createDisplay();
    
    // Piano keys
    QWidget *pianoWidget;
    QMap<int, QPushButton*> whiteKeys;
    QMap<int, QPushButton*> blackKeys;
    
    // Controls
    QSlider *volumeSlider;
    QSlider *octaveSlider;
    QComboBox *instrumentCombo;
    QLabel *volumeLabel;
    QLabel *octaveLabel;
    QLabel *displayLabel;
    
    // Status
    QLabel *statusLabel;
    
    // Style
    void applyModernStyle();
};
#endif // MAINWINDOW_H
