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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString directoryPath = "C:/Files/Project/WirenBoard_monitor/ApplicationCreate/release/inputs"; // Замените на путь к вашей директории

    int fileCount = 1; // countFilesInDirectory(directoryPath);


    //Основной функционал отправки
    for (int i = 0; i < fileCount; i++) {
        QProcess process;
        QStringList arguments;
        arguments << "-z" << "10.0.0.100" << "-T" << "-i" << "C:/Files/Project/WirenBoard_monitor/ApplicationCreate/release/release/files/input/input_file2.txt";

        process.start("zabbix_sender", arguments);


        if (process.waitForFinished()) {
            QString output = process.readAllStandardOutput();
            qDebug() << process.exitCode();
            qDebug() << output;
        }

    }


    return a.exec();
}
