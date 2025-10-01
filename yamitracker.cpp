#include "yamitracker.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QDebug>

Yamitracker::Yamitracker(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    applyModernStyle();
    setWindowTitle("Yamaha PSR-E333 Synthesizer - Yamitracker");
    setMinimumSize(1000, 600);
}

Yamitracker::~Yamitracker()
{
}

void Yamitracker::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Display section
    createDisplay();
    mainLayout->addWidget(displayLabel);
    
    // Control panel
    createControlPanel();
    mainLayout->addWidget(createControlPanel());
    
    // Piano keys
    createPianoKeys();
    mainLayout->addWidget(pianoWidget);
    
    // Status bar
    statusLabel = new QLabel("Ready");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("QLabel { background: #2c3e50; color: white; padding: 5px; }");
    mainLayout->addWidget(statusLabel);
    
    setCentralWidget(centralWidget);
}

void Yamitracker::createDisplay()
{
    displayLabel = new QLabel("Yamaha PSR-E333 - Yamitracker");
    displayLabel->setAlignment(Qt::AlignCenter);
    displayLabel->setStyleSheet(
        "QLabel {"
        "    background: linear-gradient(to right, #34495e, #2c3e50);"
        "    color: #ecf0f1;"
        "    font-size: 24px;"
        "    font-weight: bold;"
        "    padding: 15px;"
        "    border: 2px solid #34495e;"
        "    border-radius: 10px;"
        "    margin: 5px;"
        "}"
    );
}

QWidget* Yamitracker::createControlPanel()
{
    QWidget *controlWidget = new QWidget();
    QHBoxLayout *controlLayout = new QHBoxLayout(controlWidget);
    
    // Volume control
    QGroupBox *volumeGroup = new QGroupBox("Volume");
    QVBoxLayout *volumeLayout = new QVBoxLayout(volumeGroup);
    volumeLabel = new QLabel("100");
    volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setRange(0, 127);
    volumeSlider->setValue(100);
    volumeLayout->addWidget(volumeLabel);
    volumeLayout->addWidget(volumeSlider);
    
    // Instrument selection
    QGroupBox *instrumentGroup = new QGroupBox("Instrument");
    QVBoxLayout *instrumentLayout = new QVBoxLayout(instrumentGroup);
    instrumentCombo = new QComboBox();
    instrumentCombo->addItems({
        "Piano", "Electric Piano", "Organ", "Guitar", 
        "Bass", "Strings", "Brass", "Synth"
    });
    instrumentLayout->addWidget(instrumentCombo);
    
    // Octave control
    QGroupBox *octaveGroup = new QGroupBox("Octave");
    QVBoxLayout *octaveLayout = new QVBoxLayout(octaveGroup);
    octaveLabel = new QLabel("0");
    octaveSlider = new QSlider(Qt::Horizontal);
    octaveSlider->setRange(-2, 2);
    octaveSlider->setValue(0);
    octaveLayout->addWidget(octaveLabel);
    octaveLayout->addWidget(octaveSlider);
    
    controlLayout->addWidget(volumeGroup);
    controlLayout->addWidget(instrumentGroup);
    controlLayout->addWidget(octaveGroup);
    controlLayout->addStretch();
    
    // Connect signals
    connect(volumeSlider, &QSlider::valueChanged, this, &Yamitracker::onVolumeChanged);
    connect(instrumentCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &Yamitracker::onInstrumentChanged);
    connect(octaveSlider, &QSlider::valueChanged, this, &Yamitracker::onOctaveChanged);
    
    return controlWidget;
}

void Yamitracker::createPianoKeys()
{
    pianoWidget = new QWidget();
    QHBoxLayout *pianoLayout = new QHBoxLayout(pianoWidget);
    pianoLayout->setSpacing(0);
    pianoLayout->setContentsMargins(10, 10, 10, 10);
    
    // Create 25 white keys (2 octaves + 1 note)
    for (int i = 0; i < 25; i++) {
        QPushButton *whiteKey = new QPushButton();
        whiteKey->setFixedSize(40, 200);
        whiteKey->setStyleSheet(
            "QPushButton {"
            "    background: white;"
            "    border: 1px solid #bdc3c7;"
            "    border-radius: 0 0 5px 5px;"
            "}"
            "QPushButton:pressed {"
            "    background: #3498db;"
            "}"
        );
        
        // Store reference with MIDI note number (starting from C3 = 48)
        int note = 48 + i;
        whiteKeys[note] = whiteKey;
        
        connect(whiteKey, &QPushButton::pressed, [this, note]() {
            onNotePressed(note);
        });
        connect(whiteKey, &QPushButton::released, [this, note]() {
            onNoteReleased(note);
        });
        
        pianoLayout->addWidget(whiteKey);
        
        // Add black keys where appropriate
        int positionInOctave = i % 12;
        if (positionInOctave == 0 || positionInOctave == 2 || positionInOctave == 5 || 
            positionInOctave == 7 || positionInOctave == 9) {
            // These positions get black keys
            if (i < 24) { // Don't add black key after the last white key
                QPushButton *blackKey = new QPushButton();
                blackKey->setFixedSize(25, 120);
                blackKey->setStyleSheet(
                    "QPushButton {"
                    "    background: black;"
                    "    border: 1px solid #2c3e50;"
                    "    border-radius: 0 0 3px 3px;"
                    "    margin-left: -12px;"
                    "    margin-right: -12px;"
                    "    z-index: 1;"
                    "}"
                    "QPushButton:pressed {"
                    "    background: #2980b9;"
                    "}"
                );
                
                int blackNote = note + 1;
                if (positionInOctave == 2) blackNote = note + 1; // C#
                if (positionInOctave == 5) blackNote = note + 1; // F#
                if (positionInOctave == 7) blackNote = note + 1; // G#
                if (positionInOctave == 9) blackNote = note + 1; // A#
                if (positionInOctave == 0) blackNote = note + 1; // C# (next octave)
                
                blackKeys[blackNote] = blackKey;
                
                connect(blackKey, &QPushButton::pressed, [this, blackNote]() {
                    onNotePressed(blackNote);
                });
                connect(blackKey, &QPushButton::released, [this, blackNote]() {
                    onNoteReleased(blackNote);
                });
                
                pianoLayout->addWidget(blackKey);
            }
        }
    }
}

void Yamitracker::applyModernStyle()
{
    setStyleSheet(
        "QMainWindow {"
        "    background: #ecf0f1;"
        "}"
        "QGroupBox {"
        "    font-weight: bold;"
        "    border: 2px solid #bdc3c7;"
        "    border-radius: 8px;"
        "    margin-top: 1ex;"
        "    padding-top: 10px;"
        "    background: white;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    subcontrol-position: top center;"
        "    padding: 0 5px;"
        "    background: #3498db;"
        "    color: white;"
        "    border-radius: 4px;"
        "}"
        "QSlider::groove:horizontal {"
        "    border: 1px solid #bbb;"
        "    background: white;"
        "    height: 6px;"
        "    border-radius: 3px;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: #3498db;"
        "    border: 1px solid #2980b9;"
        "    width: 18px;"
        "    margin: -8px 0;"
        "    border-radius: 9px;"
        "}"
        "QComboBox {"
        "    border: 2px solid #bdc3c7;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "    background: white;"
        "    min-width: 120px;"
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "}"
        "QComboBox::down-arrow {"
        "    image: none;"
        "    border-left: 5px solid transparent;"
        "    border-right: 5px solid transparent;"
        "    border-top: 5px solid #34495e;"
        "}"
    );
}

void Yamitracker::onNotePressed(int note)
{
    // Here you would integrate with your MIDI backend
    qDebug() << "Note pressed:" << note;
    
    // Visual feedback
    if (whiteKeys.contains(note)) {
        whiteKeys[note]->setStyleSheet(
            "QPushButton {"
            "    background: #3498db;"
            "    border: 1px solid #2980b9;"
            "    border-radius: 0 0 5px 5px;"
            "}"
        );
    }
    if (blackKeys.contains(note)) {
        blackKeys[note]->setStyleSheet(
            "QPushButton {"
            "    background: #2980b9;"
            "    border: 1px solid #1c5a7d;"
            "    border-radius: 0 0 3px 3px;"
            "    margin-left: -12px;"
            "    margin-right: -12px;"
            "    z-index: 1;"
            "}"
        );
    }
    
    statusLabel->setText(QString("Playing note: %1").arg(note));
}

void Yamitracker::onNoteReleased(int note)
{
    qDebug() << "Note released:" << note;
    
    // Reset visual feedback
    if (whiteKeys.contains(note)) {
        whiteKeys[note]->setStyleSheet(
            "QPushButton {"
            "    background: white;"
            "    border: 1px solid #bdc3c7;"
            "    border-radius: 0 0 5px 5px;"
            "}"
        );
    }
    if (blackKeys.contains(note)) {
        blackKeys[note]->setStyleSheet(
            "QPushButton {"
            "    background: black;"
            "    border: 1px solid #2c3e50;"
            "    border-radius: 0 0 3px 3px;"
            "    margin-left: -12px;"
            "    margin-right: -12px;"
            "    z-index: 1;"
            "}"
        );
    }
}

void Yamitracker::onVolumeChanged(int value)
{
    volumeLabel->setText(QString::number(value));
    qDebug() << "Volume changed to:" << value;
    // Integrate with your MIDI volume control
}

void Yamitracker::onInstrumentChanged(int index)
{
    qDebug() << "Instrument changed to:" << instrumentCombo->currentText();
    displayLabel->setText(QString("Yamaha PSR-E333 - %1").arg(instrumentCombo->currentText()));
    // Integrate with your MIDI program change
}

void Yamitracker::onOctaveChanged(int value)
{
    octaveLabel->setText(QString::number(value));
    qDebug() << "Octave offset:" << value;
    // This would adjust the note mapping in your MIDI backend
}
