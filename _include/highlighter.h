#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QTextDocument;
class QTextCharFormat;
QT_END_NAMESPACE

namespace nayk {

enum HighlightSyntax { SyntaxSQL, SyntaxHTML, SyntaxCPP };

//========================================================================================================================
class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0, HighlightSyntax syntax = SyntaxSQL, bool darkBackground = false);

protected:
    void highlightBlock(const QString &text) override;

private:
    // Состояние подсветки, в которой находится текстовый блок на момент его закрытия
    enum States {
        None,       // Без подсветки
        Tag,        // Подсветка внутри тега
        Comment,    // Внутри комментария
        Quote       // Внутри кавычек, которые внутри тега
    };

    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    HighlightSyntax _syntax;
    QVector<HighlightingRule> startTagRules;    // Правила форматирования для открывающих тегов
    QVector<HighlightingRule> endTagRules;      // Правила форматирования для закрывающих тегов
    QVector<HighlightingRule> highlightingRules;
    QRegularExpression openTag;                 // Символ открыватия тега - "<"
    QRegularExpression closeTag;                // Символ закрытия тег    - ">"
    QTextCharFormat edgeTagFormat;              // Форматирование символов openTag и closeTag
    QTextCharFormat insideTagFormat;            // Форматирование текста внутри тега
    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;
    QRegularExpression quotes;                  // Регулярное выражение для текста в кавычках внутри тега
    QTextCharFormat tagsFormat;                 // Форматирование самих тегов
    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    //
    void highlightBlockHtml(const QString &text);
};
//========================================================================================================================
} // namespace
#endif // HIGHLIGHTER_H
