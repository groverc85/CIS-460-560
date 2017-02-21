#ifndef MYTREEWIDGET_H
#define MYTREEWIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <node.h>

class Node;

//In mainwindow.ui, you'll see a QTreeWidget in the GUI. That QTreeWidget
//has been "promoted" to a MyTreeWidget, which inherits from QTreeWidget
//but can implement its own functions, signals, and slots for
//our own more specific functionality

//The GUI also contains many other widgets which have NOT been promoted.
//These widgets have their built-in signals and slots linked to the
//signals and slots of our promoted classes, namely MainWindow and MyTreeWidget.

class MyTreeWidget : public QTreeWidget
{
    Q_OBJECT    //This macro tells Qt's Meta-Object Compiler to produce a source file with Qt meta-object data in it,
                //such as signal and slot data with which other Qt classes can interface.
                //In this case, the file created is moc_mainwindow.cpp, where the moc stands for meta-object compiler.
                //You should never have to make changes to this type of file, and in fact any changes you make will be reverted
                //when Qt re-compiles the class.

private:
    QString newNodeName;
    Node* selected;
public:
    MyTreeWidget(QWidget* p);

public slots:       // Slots are callable just like regular functions, but they can also be
                    // linked to signals emitted from other Q_OBJECTs, or even this Q_OBJECT itself.
                    // When linked to a signal, a slot gets called any time that signal is emitted,
                    // and it is called on the data passed by the emitted signal.
                    // For organizational purposes, I like to prefix all my slot names with slot_.


    void slot_addNode(QTreeWidgetItem*); // Stores the input node as a "top level item" in the tree.
                                         // In other words, stores it as a root node.

    void slot_storeNewNodeName(QString); // Connected to the signal emitted by the text edit box
                                         // in the GUI whenever the text in the box changes.
                                         // Stores the new next in the newNodeName member variable.

    void slot_storeSelectedNode(QTreeWidgetItem*); // Connected to the signal emitted by this QTreeWidget
                                                   // whenever an item in the widget is clicked. Stores the
                                                   // pointer to the clicked Node in the "selected" member variable

    void slot_addChildToSelected(); // Connected to the signal emitted by the push button in the GUI.
                                    // Adds a new Node (with a name taken from newNodeName) as a child to the
                                    // currently selected node.
};

#endif // MYTREEWIDGET_H
