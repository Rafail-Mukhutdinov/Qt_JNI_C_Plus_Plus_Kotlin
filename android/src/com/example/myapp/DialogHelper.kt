package com.example.myapp

import android.app.AlertDialog
import android.content.Context
import android.os.Handler
import android.os.Looper
import android.util.Log

object DialogHelper {
    @JvmStatic
    fun showDialog(context: Context) {
        Log.d("DialogHelper", "Метод showDialog вызван")

        // Используем Handler и Looper для вызова диалога на главном потоке
        Handler(Looper.getMainLooper()).post {
            AlertDialog.Builder(context)
                .setTitle("Привет от Kotlin")
                .setMessage("Это диалоговое окно, вызванное из C++")
                .setPositiveButton("ОК") { dialog, _ ->
                    Log.d("DialogHelper", "Кнопка OK нажата")
                    dialog.dismiss()
                }
                .show()
        }
    }
}
