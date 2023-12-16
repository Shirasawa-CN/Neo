/// 负责定义编辑器的基本函数和功能

#ifndef EDITORBASE_H
#define EDITORBASE_H

#include <QString>
#include <QTextEdit>
#include <QCompleter>
#include <QTextFormat>
#include <QTextCharFormat>

#include "programlanguage.h"

class EditorBase : public QTextEdit{
private:
    QString *title;
    __int128_t *id;

    ProgramLanguageSupport *language;
    
    /// status
    bool *is_saved;
    bool *is_focused;
    bool *is_modified;
    //渲染情况,如果修改后需重新渲染
    bool *is_highlighted;

public:
    EditorBase(QString title,__int128_t id,ProgramLanguageSupport language);
    EditorBase(QString title, __int128_t id);

    bool highlight();

    ~EditorBase();
};

#endif