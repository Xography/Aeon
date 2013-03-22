/*******************************************************************************
 *
 * This file is part of the AEON Framework.
 *
 * -----------------------------------------------------------------------------
 * Copyright (C) 2012- The Aeon Development Team.
 *
 * See LICENSE.txt for licensing details.
 *
 ******************************************************************************/
#ifndef ParameterNameValidator_h__
#define ParameterNameValidator_h__

#include <QValidator>
#include <QTextCodec>

/**
 * This validator validates input against a list of names (also checks for proper latin encoding)
 */
class ParameterNameValidator : public QValidator
{
	Q_OBJECT
public:	
	/**
	 * Constructor.
	 */
	ParameterNameValidator(QWidget* parent, const QList<QString>& invalidNames);

public: 
	// Implements QValidator
	QValidator::State validate(QString& input, int& pos) const override;

signals:
	/**
	 * This signal is emitted whenever the state of this validator changes, so the receiver can take appropriate action.
	 */
	void ValidationStatusChanged(QWidget* parent, QValidator::State state) const;

private:
	/**
	 * The list of invalid names
	 */
	QList<QString> invalidNames;

	/**
	 * The parent this validator is attached to.
	 */
	QWidget* parent;
};

#endif // ParameterNameValidator_h__
