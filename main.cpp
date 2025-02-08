#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtCore/QJniObject>
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qDebug() << "Приложение запускается...";

#ifdef Q_OS_ANDROID
    qDebug() << "Запуск на Android";

    // Получаем ссылку на текущую Activity
    QJniObject activity = QNativeInterface::QAndroidApplication::context();

    if (activity.isValid()) {
        qDebug() << "Activity получена успешно";

        // Вызываем статический метод showDialog
        QJniObject::callStaticMethod<void>(
            "com/example/myapp/DialogHelper",
            "showDialog",
            "(Landroid/content/Context;)V",
            activity.object<jobject>()
            );

        qDebug() << "Метод showDialog вызван";
    } else {
        qDebug() << "Не удалось получить Activity";
    }
#endif

    // Запуск QML движка
    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/main.qml"_s);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);
    engine.load(url);

    qDebug() << "Загрузка QML завершена";

    return app.exec();
}
