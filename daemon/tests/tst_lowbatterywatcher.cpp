#include <QTest>
#include "../lowbatterywatcher.hpp"

class TestLowBatteryWatcher : public QObject {
    Q_OBJECT

    static int stageOf(const std::optional<LowBatteryAlert> &a) { return a ? a->stage : -1; }

private slots:
    void alertsAt20Then10Then5() {
        LowBatteryWatcher w;
        QVERIFY(!w.evaluate(21, true, false));
        const auto first = w.evaluate(20, true, false);
        QCOMPARE(stageOf(first), 20);
        QCOMPARE(int(first->level), 20);
        const auto second = w.evaluate(10, true, false);
        QCOMPARE(stageOf(second), 10);
        QCOMPARE(int(second->level), 10);
        const auto third = w.evaluate(4, true, false);
        QCOMPARE(stageOf(third), 5);
        QCOMPARE(int(third->level), 4);
        QVERIFY(!w.evaluate(3, true, false));
    }

    void skipFrom22To8GivesOneAlertAtStage10AndMarks20Done() {
        LowBatteryWatcher w;
        QVERIFY(!w.evaluate(22, true, false));
        const auto a = w.evaluate(8, true, false);
        QCOMPARE(stageOf(a), 10);
        QCOMPARE(int(a->level), 8);
        // Stage 20 is done: a rise to 19 and back to 18 stays quiet.
        QVERIFY(!w.evaluate(19, true, false));
        QVERIFY(!w.evaluate(18, true, false));
        QVERIFY(!w.evaluate(9, true, false));
    }

    void firstReadingAlreadyDeepGivesOneAlert() {
        LowBatteryWatcher w;
        QCOMPARE(stageOf(w.evaluate(3, true, false)), 5);
        QVERIFY(!w.evaluate(3, true, false));
    }

    void hoveringAt19And20AlertsOnce() {
        LowBatteryWatcher w;
        QCOMPARE(stageOf(w.evaluate(19, true, false)), 20);
        QVERIFY(!w.evaluate(20, true, false));
        QVERIFY(!w.evaluate(19, true, false));
        QVERIFY(!w.evaluate(20, true, false));
    }

    void reArmsAt25() {
        LowBatteryWatcher w;
        QVERIFY(w.evaluate(4, true, false));
        QVERIFY(!w.evaluate(24, true, false));
        QVERIFY(!w.evaluate(19, true, false));  // 24 did not re-arm
        QVERIFY(!w.evaluate(25, true, false));
        QCOMPARE(stageOf(w.evaluate(20, true, false)), 20);
    }

    void reArmsOnCharging() {
        LowBatteryWatcher w;
        QVERIFY(w.evaluate(7, true, false));
        QVERIFY(!w.evaluate(7, true, true));  // charging alerts nothing
        QCOMPARE(stageOf(w.evaluate(7, true, false)), 10);
    }

    void unavailableChangesNothing() {
        LowBatteryWatcher w;
        QVERIFY(!w.evaluate(0, false, false));  // no alert from a missing pod
        QCOMPARE(stageOf(w.evaluate(8, true, false)), 10);
        QVERIFY(!w.evaluate(30, false, false));  // does not re-arm
        QVERIFY(!w.evaluate(8, false, true));    // charging flag ignored too
        QVERIFY(!w.evaluate(8, true, false));
    }
};

QTEST_GUILESS_MAIN(TestLowBatteryWatcher)
#include "tst_lowbatterywatcher.moc"
