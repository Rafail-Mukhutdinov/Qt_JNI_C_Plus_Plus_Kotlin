#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtCore/QJniObject>
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <jni.h>

// Функция для вызова из Java
extern "C" JNIEXPORT void JNICALL
Java_com_example_myapp_DialogHelper_callCppCallback(JNIEnv *env, jobject, jlong callback, jboolean result) {
    auto cb = reinterpret_cast<std::function<void(bool)> *>(callback);
    (*cb)(result);
    delete cb;
}

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

        // Создаем callback функцию
        auto callback = new std::function<void(bool)>([](bool result) {
            if (result) {
                qDebug() << "Пользователь нажал ОК";
            } else {
                qDebug() << "Пользователь нажал Отмена";
            }
        });

        // Сообщение для отображения в диалоговом окне
        QString message = "Привет Это диалоговое окно, вызванное из C++";

        // Вызываем статический метод showDialog с передачей текста и callback функции
        QJniObject::callStaticMethod<void>(
            "com/example/myapp/DialogHelper",
            "showDialog",
            "(Landroid/content/Context;Ljava/lang/String;J)V",
            activity.object<jobject>(),
            QJniObject::fromString(message).object<jstring>(),
            reinterpret_cast<jlong>(callback)
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
