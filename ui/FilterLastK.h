#pragma once
#include <QSortFilterProxyModel>

/* Прокси, оставляющий только первые K строк (таблица уже отсортирована DESC) */
class FilterLastK : public QSortFilterProxyModel
{
public:
    explicit FilterLastK(QObject *parent = nullptr)
        : QSortFilterProxyModel(parent) {}

    void setLastK(int k) { m_lastK = k; invalidateFilter(); }

protected:
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &) const override
    {
        return (m_lastK == 0) || (sourceRow < m_lastK);
    }
private:
    int m_lastK = 0;          // 0 ⇒ показываем все
};
