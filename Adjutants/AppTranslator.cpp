#include "AppTranslator.h"


AppTranslator::AppTranslator(const QString &pathTranslationFile, QObject *parent) :
    QObject(parent)
{
    // 0.
    /*      QApplication a(argc, argv);
            a.setApplicationVersion(APP_VERSION);
            AppTranslator appTranslator(FileManager::getLanguageApp(), "Resources/translationFiles");
    */
    // 1. add to (.pro) file:
    /*    TRANSLATIONS += \
              translationFiles/appTranslator_ru_UA.ts \
              translationFiles/appTranslator_ru_RU.ts
    */
    //    OTHER_FILES += $${TRANSLATIONS}
    // 2. QtCreator -> Tools -> External -> Linguist -> Update Translations (lupdate)
    // 3. run ./home/jp/Qt/5.15.0/gcc_64/bin/linguist
    // 4. QtCreator -> Tools -> External -> Linguist -> Release Translations (lrelease)
    // 5. create translationFiles.qrc and add appTranslator_ru_UA.qm, appTranslator_ru_RU.qm
    /*    RESOURCES += \
              translationFiles.qrc
    */


    // pathTranslationFile == :/translationFiles/translationFiles/appTranslator_ru_UA.qm
    loadTranslator(pathTranslationFile);
}

void AppTranslator::setNewLanguage(const QString &pathTranslationFile)
{
    loadTranslator(pathTranslationFile);
}

void AppTranslator::loadTranslator(const QString &file)
{
    static QTranslator translator;
    QApplication::removeTranslator(&translator);
    translator.load(file);
    QApplication::installTranslator(&translator);
    testTranslator();
}

void AppTranslator::testTranslator()
{
    qDebug() << "translation for Hello world - " << QObject::tr("Hello world") << Qt::endl;
    // 2. QtCreator -> Tools -> External -> Linguist -> Update Translations (lupdate)
    // 3. run ./home/jp/Qt/5.15.0/gcc_64/bin/linguist
    // 3. or replace
    //        <source>Hello world</source>
    //        <translation type="unfinished"></translation>
    //
    //        <source>Hello world</source>
    //        <translation>Приветик мир!</translation>
    // 4. QtCreator -> Tools -> External -> Linguist -> Release Translations (lrelease)
}
