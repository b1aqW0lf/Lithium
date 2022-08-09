#include "select_source_ui.h"

SelectSourceUI::SelectSourceUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectSourceUI)
{
    ui->setupUi(this);

    //connect signals and slots
    connect(ui->sourceBrowse1, &QPushButton::clicked,
            this, &SelectSourceUI::select_input1);
    connect(ui->sourceBrowse2, &QPushButton::clicked,
            this, &SelectSourceUI::select_input2);

    //interface buttons initialization
    ui->sourceBrowse1 = new QPushButton(tr("..."));
    ui->sourceBrowse2 = new QPushButton(tr("..."));

    //source1 input field default settings
    ui->sourceInput1Edit->setToolTip(tr("Select video file to process"));
    ui->sourceInput1Edit->setPlaceholderText(tr("Select video file to process"));
    //ui->source1Label->setTextFormat(Qt::RichText);//it works
    ui->source1Label->setText(tr("Source 1:"));//use HTML formating to bold text

    //source2 input field default settings
    ui->sourceInput2Edit->setEnabled(false);
    ui->sourceBrowse2->setEnabled(false);
    ui->source2Label->setEnabled(false);
    ui->source2Label->setText(tr("Source 2:"));

    //groupbox
    ui->InputGroupBox->setTitle(tr("Select Sources "));
    ui->InputGroupBox->setAlignment(Qt::AlignLeft);
}

SelectSourceUI::~SelectSourceUI()
{
    delete ui;
}

//select first input file to convert
void SelectSourceUI::select_input1()//complete!
{
    input_file1 =
            QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C:/input_file1",
                tr("Videos (*.mp4 *.mov *.avi *.mkv *.webm *.ogv *.mpeg)"));
    if(!input_file1.isEmpty())
    {
        ui->sourceInput1Edit->setText(input_file1);
    }
}

//select optional second input, an audio file to use
void SelectSourceUI::select_input2()
{
    input_file2 =
            QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C:/input_file2",
                tr("Audio (*.wav *.mp4a *.m4a *.flac *.mp3 *.ogg *.3gp)"));
    if(!input_file2.isEmpty())
    {
        ui->sourceInput2Edit->setText(input_file2);
    }
}

