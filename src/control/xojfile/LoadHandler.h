/*
 * Xournal++
 *
 * Loads an .xoj / .xopp document
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include "model/Document.h"
#include "model/Image.h"
#include "model/Stroke.h"
#include "model/TexImage.h"
#include "model/Text.h"

#include <XournalType.h>

#include <zlib.h>

enum ParserPosition
{
    PARSER_POS_NOT_STARTED = 1,	// Waiting for opening <xounal> tag
    PARSER_POS_STARTED,			// Waiting for Metainfo or contents like <page>
    PARSER_POS_IN_PAGE,			// Starting page tag read
    PARSER_POS_IN_LAYER,		// Starting layer tag read
    PARSER_POS_IN_STROKE,		// Starting layer tag read
    PARSER_POS_IN_TEXT,			// Starting text tag read
    PARSER_POS_IN_IMAGE,		// Starting image tag read
    PARSER_POS_IN_TEXIMAGE,		// Starting latex tag read

    PASER_POS_FINISHED			// Document is parsed
};

class LoadHandler
{
public:
	LoadHandler();
	virtual ~LoadHandler();

public:
	Document* loadDocument(string filename);

	string getLastError();
	bool isAttachedPdfMissing();
	string getMissingPdfFilename();

	void removePdfBackground();
	void setPdfReplacement(string filename, bool attachToDocument);

private:
	void parseStart();
	void parseContents();
	void parsePage();
	void parseLayer();

	void parseStroke();
	void parseText();
	void parseImage();
	void parseTexImage();

private:
	void initAttributes();

	string readLine();
	int readFile(char* buffer, int len);
	bool closeFile();
	bool openFile(string filename);
	bool parseXml();

	static void parserText(GMarkupParseContext* context, const gchar* text, gsize text_len, gpointer userdata,
	                       GError** error);
	static void parserEndElement(GMarkupParseContext* context, const gchar* element_name, gpointer userdata,
	                             GError** error);
	static void parserStartElement(GMarkupParseContext* context, const gchar* element_name,
	                               const gchar** attribute_names, const gchar** attribute_values,
	                               gpointer userdata, GError** error);

	const char* getAttrib(const char* name, bool optional = false);
	double getAttribDouble(const char* name);
	int getAttribInt(const char* name);

	void parseBgSolid();
	void parseBgPixmap();
	void parseBgPdf();

	void readImage(const gchar* base64_str, gsize base64_strlen);
	void readTexImage(const gchar* base64_str, gsize base64_strlen);

private:
	XOJ_TYPE_ATTRIB;

	string lastError;
	string pdfMissing;
	bool attachedPdfMissing;

	bool removePdfBackgroundFlag;
	string pdfReplacementFilename;
	bool pdfReplacementAttach;

	string filename;

	bool pdfFilenameParsed;

	ParserPosition pos;

	string creator;
	int fileversion;

	gzFile fp;

	vector<double> pressureBuffer;

	PageRef page;
	Layer* layer;
	Stroke* stroke;
	Text* text;
	Image* image;
	TexImage* teximage;

	const char* endRootTag = "xournal";

	string xournalFilename;

	GError* error;
	const gchar** attributeNames;
	const gchar** attributeValues;
	const gchar* elementName;

	int loadedTimeStamp;
	string loadedFilename;

	DocumentHandler dHanlder;
	Document doc;

	friend class LoadHandlerHelper;
};

