#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QProcess>

int countFilesInDirectory(const QString &path) {
    QDir dir(path);
    int count = 0;

    // Устанавливаем фильтр для поиска всех файлов и директорий, исключая специальные папки "." и ".."
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    // Получаем список всех файлов и папок
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.isDir()) {
            // Если это директория, рекурсивно вызываем функцию
            count += countFilesInDirectory(fileInfo.absoluteFilePath());
        } else {
            // Если это файл, увеличиваем счетчик
            ++count;
        }
    }

    return count;
}


QVector<QString> getAllFileNames(const QString &path) {
    QDir dir(path);
    QVector<QString> fileNames;

    // Устанавливаем фильтр для поиска всех файлов и директорий, исключая специальные папки "." и ".."
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    // Получаем список всех файлов и папок
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.isDir()) {
            // Если это директория, рекурсивно вызываем функцию и добавляем результаты
            QVector<QString> subdirFiles = getAllFileNames(fileInfo.absoluteFilePath());
            fileNames += subdirFiles; // Добавляем имена файлов из поддиректорий
        } else {
            // Если это файл, добавляем его имя в вектор
            fileNames.append(fileInfo.fileName());
        }
    }

    return fileNames;
}


bool removeFile(const QString &filePath) {
    if (QFile::exists(filePath)) { // Проверяем, существует ли файл
        bool success = QFile::remove(filePath);
        return success;
    } else {
        qDebug() << "File does not exist:" << filePath;
        return false;
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString directoryPath = "C:/Files/Project/WirenBoard_monitor/ApplicationCreate/release/release/files/input"; // Замените на путь к вашей директории

    int fileCount = countFilesInDirectory(directoryPath);

    qDebug() << fileCount;

    QVector<QString> fileNames = getAllFileNames(directoryPath);

    //Основной функционал отправки
    for (int i = 0; i < fileCount; i++) {
        QProcess process;
        QStringList arguments;
        QString path = "C:/Files/Project/WirenBoard_monitor/ApplicationCreate/release/release/files/input";
        QString pathtoinputs = QString( path + "/%1").arg(fileNames[i]);

        arguments << "-z" << "10.0.0.100" << "-T" << "-i" << pathtoinputs;

        process.start("zabbix_sender", arguments);

        if (process.waitForFinished()) {
            QString output = process.readAllStandardOutput();
            if (process.exitCode() == 0) {
                qDebug() << "Sending the file to the server is successful:" << pathtoinputs;
                bool result = removeFile(pathtoinputs);
                if (result) {
                   qDebug() << "File successfully removed:" << pathtoinputs;
                } else {
                   qDebug() << "Failed to remove file:" << pathtoinputs;
                }
            }
            else {
                qDebug() << "Error sending the file to the server:" << pathtoinputs;
            }
        }

    }

    return 0;

    return a.exec();
}
