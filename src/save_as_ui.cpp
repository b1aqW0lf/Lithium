#include "save_as_ui.h"
#include "ui_save_as_ui.h"

SaveAsUI::SaveAsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaveAsUI)
{
    ui->setupUi(this);

    //connect signals and slots
    connect(ui->saveASButton, SIGNAL(clicked()), this, SLOT(select_output()));

    //saveAS line edit
    //ui->saveASEdit;

    //saveAS label
    ui->saveASLabel->setText(tr("Save As: "));

    //saveAS groupbox
    ui->OutputGroupBox->setTitle(tr("Select Destination "));
    ui->OutputGroupBox->setAlignment(Qt::AlignLeft);
}

SaveAsUI::~SaveAsUI()
{
    delete ui;
}

//select the output file to convert to
void SaveAsUI::select_output()
{
    output_file =
            QFileDialog::getSaveFileName(
                this,
                tr("Save File"),
                "C:/output_file",
                tr(""));
    if(!output_file.isEmpty())
    {
        ui->saveASEdit->setText(output_file);
    }
    emit saveas_value();
}
