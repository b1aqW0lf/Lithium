#include "process_mode_ui.h"
#include "ui_process_mode_ui.h"

ProcessModeUI::ProcessModeUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcessModeUI)
{
    ui->setupUi(this);

    //connect signals and slots
    /*connect(ui->mergeSourcesRadio, &QRadioButton::clicked,
               this, &ProcessModeUI::merge_sources);
    connect(ui->extractAudioRadio, &QRadioButton::clicked,
            this, &ProcessModeUI::extract_audio);
    connect(ui->normalModeRadio, &QRadioButton::clicked,
            this, &ProcessModeUI::normal_mode);*/
    //--------------------------------------------------------------------------

    //Initialize interface widgets
    //radio buttons defaults
    processButtons = new QButtonGroup(this);
    processButtons->addButton(ui->normalModeRadio);
    processButtons->addButton(ui->mergeSourcesRadio);
    processButtons->addButton(ui->extractAudioRadio);
    processButtons->setExclusive(true);
    ui->normalModeRadio->setChecked(true);
    ui->normalModeRadio->setToolTip(tr("Normal processing"));
    ui->extractAudioRadio->setToolTip(tr("Extract audio from video to save"));
    ui->mergeSourcesRadio->setToolTip(tr("Merge a video source with an audio "
                                  "source"));
    ui->modeGroupBox->setTitle(tr("Process Mode"));
    ui->modeGroupBox->setAlignment(Qt::AlignLeft);
    //--------------------------------------------------------------------------
}

ProcessModeUI::~ProcessModeUI()
{
    delete ui;
}
/*
void ProcessModeUI::merge_sources()
{
    //enable the Source 2 fields
    ui->sourceInput2Edit->setEnabled(true);
    ui->sourceBrowse2->setEnabled(true);
    ui->source2Label->setEnabled(true);
    ui->sourceInput1Edit->setToolTip(tr("Select video source to merge"));
    ui->sourceInput2Edit->setToolTip(tr("Select audio source to merge "
                                                "with video source"));
    //merging sources 1 and 2 is done in encoding_started()
}

void ProcessModeUI::extract_audio()
{
    //disable the Source 2 fields
    ui->sourceInput2Edit->setDisabled(true);
    ui->sourceBrowse2->setDisabled(true);
    ui->source2Label->setDisabled(true);
    ui->sourceInput2Edit->setToolTip("");//turn off source 2 tool tip
    ui->sourceInput1Edit->setToolTip(tr("Select video file in Source 1 field"
                                     " to extract an audio stream from"));
    //audio extraction prcoessing in is done in conversion_prep()
}

void ProcessModeUI::normal_mode()
{
    //set default Source 2 field settings -- disable Source 2 fields
    ui->sourceInput2Edit->setEnabled(false);
    ui->sourceBrowse2->setEnabled(false);
    ui->source2Label->setEnabled(false);
    ui->sourceInput2Edit->setToolTip("");//turn off source 2 tool tip
    ui->sourceInput1Edit->setToolTip(tr("Select video file to process"));
    //normal mode prcoessing in is done in conversion_prep()
}

}*/
