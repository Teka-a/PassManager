#include "mainwindow.h"
#include "form_hashvalidationfailed.h"

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>

#include <QApplication>
#include <QStyleFactory>

#include <sys/ptrace.h>
#include <errno.h>
#include <stdlib.h>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <stdio.h>


__attribute__((section(".ref_checksum"))) unsigned char REFERENCE_CHECKSUM[32] = {
    0x6f, 0x33, 0x5e, 0xbb, 0xcb, 0x17, 0x83, 0xd3,
    0x67, 0xf8, 0xc7, 0xcc, 0x55, 0xf1, 0x6a, 0x4a,
    0x97, 0x2a, 0xad, 0xd6, 0x51, 0x2e, 0xbb, 0xa6,
    0xb6, 0xc0, 0x1b, 0xd4, 0xe9, 0x22, 0x03, 0x33
};

bool getTextSegmentInfo(const QString& filePath, off_t* textOffset, size_t* textSize) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file:" << filePath;
        return false;
    }

    Elf64_Ehdr elfHeader;
    if (file.read(reinterpret_cast<char*>(&elfHeader), sizeof(elfHeader)) != sizeof(elfHeader)) {
        qDebug() << "Failed to read ELF header";
        return false;
    }

    if (memcmp(elfHeader.e_ident, ELFMAG, SELFMAG) != 0) {
        qDebug() << "Not an ELF file";
        return false;
    }

    file.seek(elfHeader.e_shoff + elfHeader.e_shstrndx * sizeof(Elf64_Shdr));
    Elf64_Shdr strTabHeader;
    if (file.read(reinterpret_cast<char*>(&strTabHeader), sizeof(strTabHeader)) != sizeof(strTabHeader)) {
        qDebug() << "Failed to read section header string table";
        return false;
    }

    QByteArray strTable;
    file.seek(strTabHeader.sh_offset);
    strTable.resize(strTabHeader.sh_size);
    if (file.read(strTable.data(), strTabHeader.sh_size) != strTabHeader.sh_size) {
        qDebug() << "Failed to read section name string table";
        return false;
    }

    file.seek(elfHeader.e_shoff);
    for (int i = 0; i < elfHeader.e_shnum; i++) {
        Elf64_Shdr sectionHeader;
        if (file.read(reinterpret_cast<char*>(&sectionHeader), sizeof(sectionHeader)) != sizeof(sectionHeader)) {
            qDebug() << "Failed to read section header";
            return false;
        }

        const char* sectionName = strTable.data() + sectionHeader.sh_name;
        if (QString(sectionName) == ".text") {
            *textOffset = sectionHeader.sh_offset;
            *textSize = sectionHeader.sh_size;
            qDebug() << ".text offset:" << Qt::hex << *textOffset << "size:" << Qt::hex << *textSize;
            file.close();
            return true;
        }
    }

    qDebug() << "Text section not found";
    file.close();
    return false;
}

QByteArray calculateChecksum(off_t textOffset, size_t textSize) {
    if (textSize == 0) {
        qDebug() << "Invalid text size";
        return QByteArray();
    }

    int fd = open(QCoreApplication::applicationFilePath().toStdString().c_str(), O_RDONLY);
    if (fd == -1) {
        qDebug() << "Failed to open executable:" << strerror(errno);
        return QByteArray();
    }

    // Выравниваем смещение и размер по границе страницы
    const size_t pageSize = sysconf(_SC_PAGE_SIZE); // Обычно 4096
    off_t alignedOffset = textOffset & ~(pageSize - 1); // Округляем вниз до страницы
    size_t alignedSize = ((textOffset + textSize - alignedOffset) + pageSize - 1) & ~(pageSize - 1); // Округляем вверх

    qDebug() << "Original offset:" << Qt::hex << textOffset << "size:" << textSize;
    qDebug() << "Aligned offset:" << Qt::hex << alignedOffset << "size:" << alignedSize;

    void* mapped = mmap(nullptr, alignedSize, PROT_READ, MAP_PRIVATE, fd, alignedOffset);
    if (mapped == MAP_FAILED) {
        qDebug() << "Failed to map file:" << strerror(errno);
        close(fd);
        return QByteArray();
    }

    // Смещение внутри отображённой области
    off_t offsetInMapping = textOffset - alignedOffset;
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData((const char*)mapped + offsetInMapping, textSize);
    QByteArray result = hash.result();

    munmap(mapped, alignedSize);
    close(fd);

    return result;
}

bool checkIntegrity() {
    off_t textOffset = 0;
    size_t textSize = 0;

    if (!getTextSegmentInfo(QCoreApplication::applicationFilePath(), &textOffset, &textSize)) {
        qDebug() << "Failed to get text segment info";
        return false;
    }

    QByteArray currentChecksum = calculateChecksum(textOffset, textSize);
    if (currentChecksum.isEmpty()) {
        qDebug() << "Failed to calculate checksum";
        return false;
    }

    QByteArray refChecksum((const char*)REFERENCE_CHECKSUM, 32);

    if (currentChecksum != refChecksum) {
        qDebug() << "WARNING: Application integrity check failed!";
        qDebug() << "Expected:" << refChecksum.toHex();
        qDebug() << "Got:" << currentChecksum.toHex();
        return false;
    }

    qDebug() << "Integrity check passed";
    return true;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);





    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) {
        if (errno == EPERM) {
            qDebug() << "Debugger attached!";
        } else {
            qDebug() << "no debugger attached!";
        }
    }

    qApp->setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(11, 3, 23));
    darkPalette.setColor(QPalette::WindowText, QColor(196, 196, 196));
    darkPalette.setColor(QPalette::Base, Qt::white);
    darkPalette.setColor(QPalette::AlternateBase, Qt::gray);
    darkPalette.setColor(QPalette::ToolTipBase, QColor(65, 63, 167));
    darkPalette.setColor(QPalette::ToolTipText, QColor(65, 63, 167));
    darkPalette.setColor(QPalette::Text, Qt::gray);
    darkPalette.setColor(QPalette::ButtonText, Qt::gray);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(65, 63, 167));
    darkPalette.setColor(QPalette::Highlight, QColor(65, 63, 167));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);

    qApp->setPalette(darkPalette);

    qApp->setStyleSheet("QPushButton { border: none; background: transparent; } "
                        "QLineEdit { border: none; border-bottom: 1px solid #C4C4C4; padding: 5px; background: transparent;  } "
                        "QLabel {font: 12px; } "
                        "QLineEdit:focus { border-bottom: 1px solid #413FA7; }"
                        "QPushButton:hover { border-bottom: 3px solid #0B0317; padding: 5px; }"
                        "QLineEdit[text=\"\"] {color: gray;}"
                        );

    Form_HashValidationFailed failed;
    MainWindow w;


    if (!checkIntegrity()) {
        qDebug() << "Integrity failed!";
        failed.show();
    } else {
        w.show();
    }



    return a.exec();
}
