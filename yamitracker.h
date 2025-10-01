#ifndef YAMITRACKER_H
#define YAMITRACKER_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QGridLayout>
#include <QWidget>
#include <QGroupBox>
#include <QMap>

class Yamitracker : public QMainWindow
{
    Q_OBJECT

public:
    Yamitracker(QWidget *parent = nullptr);
    ~Yamitracker();

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
#endif // YAMITRACKER_H
