package com.example.myapp

import android.app.AlertDialog
import android.content.Context
import android.os.Handler
import android.os.Looper
import android.util.Log

object DialogHelper {
    @JvmStatic
    fun showDialog(context: Context, message: String, callback: Long) {
        Log.d("DialogHelper", "Метод showDialog вызван")

        // Используем Handler и Looper для вызова диалога на главном потоке
        Handler(Looper.getMainLooper()).post {
            AlertDialog.Builder(context)
                .setTitle("Привет от Kotlin")
                .setMessage(message)
                .setPositiveButton("ОК") { dialog, _ ->
                    Log.d("DialogHelper", "Кнопка OK нажата")
                    callCppCallback(callback, true)
                    dialog.dismiss()
                }
                .setNegativeButton("Отмена") { dialog, _ ->
                    Log.d("DialogHelper", "Кнопка Отмена нажата")
                    callCppCallback(callback, false)
                    dialog.dismiss()
                }
                .show()
        }
    }

    private external fun callCppCallback(callback: Long, result: Boolean)
}
