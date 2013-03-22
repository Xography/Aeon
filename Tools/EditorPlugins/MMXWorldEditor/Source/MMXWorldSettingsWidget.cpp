/*******************************************************************************
 *
 * This file is part of Sample Game.
 *
 * -----------------------------------------------------------------------------
 * Copyright (C) 2012- The Aeon Development Team.
 *
 * See LICENSE.txt for licensing details.
 *
 ******************************************************************************/

 #include "AeonWorldSettingsWidget.h"

// ----------------------------------------------------------------------------
AeonSettingsWidget::AeonSettingsWidget(QWidget* parent, AeonEditorWorld* world) :
	QWidget(parent),
	world(world)
{
	ui.setupUi(this);

	connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton* )), this, SLOT(OnButtonClicked(QAbstractButton*)));

	OnReset();
}

// ----------------------------------------------------------------------------
void AeonSettingsWidget::OnButtonClicked(QAbstractButton* btn)
{
	if(ui.buttonBox->standardButton(btn) == QDialogButtonBox::Save)
	{
		OnSave();
	}

	if(ui.buttonBox->standardButton(btn) == QDialogButtonBox::Reset)
	{
		OnReset();
	}
}

// ----------------------------------------------------------------------------
void AeonSettingsWidget::OnSave()
{
	if(!world) return;

	world->SetWorldName(ui.lineEditWorldName->text().toAscii().data());
	world->GetSettings()->SetAttribute<Aeon::String>(ATTR("VEngine World File"), ui.lineEditWorldFile->text().toAscii().data());
	world->GetSettings()->SetAttribute<Aeon::String>(ATTR("Soundtrack ID"), ui.lineEditSoundtrackID->text().toAscii().data());
	world->GetSettings()->SetAttribute<bool>(ATTR("Layered Track"), ui.radioButtonLayered->isChecked());

	world->Save(world->GetFilename());
}

// ----------------------------------------------------------------------------
void AeonSettingsWidget::OnReset()
{
	if(!world) return;

	Aeon::Entity* settings = world->GetSettings();

	if(!settings)
	{
		QMessageBox::warning(this, tr("Invalid Settings"), tr("World settings invalid, cannot read fields. You will have to fill in the data again."));
		return;
	}

	ui.lineEditWorldName->setText(world->GetWorldName().c_str());

	if(!CheckAttribute(settings, "VEngine World File", Aeon::IAttribute::STRING)) return;
	ui.lineEditWorldFile->setText(QString(settings->GetAttribute<Aeon::String>(ATTR("VEngine World File")).c_str()));

	if(!CheckAttribute(settings, "Layered Track", Aeon::IAttribute::BOOL)) return;
	ui.radioButtonLayered->setChecked(settings->GetAttribute<bool>(ATTR("Layered Track")));

	if(!CheckAttribute(settings, "Soundtrack ID", Aeon::IAttribute::STRING)) return;
	ui.lineEditSoundtrackID->setText(QString(settings->GetAttribute<Aeon::String>(ATTR("Soundtrack ID")).c_str()));
}

// ----------------------------------------------------------------------------
bool AeonSettingsWidget::CheckAttribute(Aeon::Entity* settings, const Aeon::String& name, Aeon::IAttribute::Type type)
{
	if(settings->CheckAttribute(ATTR(name)) != type)
	{	
		LOG_DEBUG << "type: " << type << " <-> " << settings->CheckAttribute(ATTR(name));
		QMessageBox::warning(this, tr("File inconsistencies detected"), tr("Expected field \"%1\" "
			"to be present and of valid type; the entity descriptor appears to be incompatible.").arg(name.c_str()));
		return false;
	}

	return true;
}

// -------------------------------------------------------------------------
void AeonSettingsWidget::SetWorld(AeonEditorWorld* world)
{
	this->world = world;
	OnReset();
}
