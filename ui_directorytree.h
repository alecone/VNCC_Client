/********************************************************************************
** Form generated from reading UI file 'directorytree.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIRECTORYTREE_H
#define UI_DIRECTORYTREE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TreeDir
{
public:
    QTreeWidget *treeWidget;
    QPushButton *downloadButton;
    QPushButton *removeButton;
    QPushButton *uploadButton;

    void setupUi(QWidget *TreeDir)
    {
        if (TreeDir->objectName().isEmpty())
            TreeDir->setObjectName(QStringLiteral("TreeDir"));
        TreeDir->setWindowModality(Qt::WindowModal);
        TreeDir->setEnabled(true);
        TreeDir->resize(562, 387);
        TreeDir->setFocusPolicy(Qt::NoFocus);
        treeWidget = new QTreeWidget(TreeDir);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(0, 0, 561, 321));
        treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeWidget->setAnimated(true);
        treeWidget->setColumnCount(1);
        downloadButton = new QPushButton(TreeDir);
        downloadButton->setObjectName(QStringLiteral("downloadButton"));
        downloadButton->setGeometry(QRect(60, 340, 93, 28));
        removeButton = new QPushButton(TreeDir);
        removeButton->setObjectName(QStringLiteral("removeButton"));
        removeButton->setGeometry(QRect(420, 340, 93, 28));
        removeButton->setFlat(false);
        uploadButton = new QPushButton(TreeDir);
        uploadButton->setObjectName(QStringLiteral("uploadButton"));
        uploadButton->setGeometry(QRect(240, 340, 93, 28));

        retranslateUi(TreeDir);

        QMetaObject::connectSlotsByName(TreeDir);
    } // setupUi

    void retranslateUi(QWidget *TreeDir)
    {
        TreeDir->setWindowTitle(QApplication::translate("TreeDir", "Your Cloud Storage", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("TreeDir", "Name", nullptr));
        downloadButton->setText(QApplication::translate("TreeDir", "Download", nullptr));
        removeButton->setText(QApplication::translate("TreeDir", "Remove", nullptr));
        uploadButton->setText(QApplication::translate("TreeDir", "Upload", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TreeDir: public Ui_TreeDir {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIRECTORYTREE_H
