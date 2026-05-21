package im.vector.app.features.alarm

import android.app.AlarmManager
import android.app.PendingIntent
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import chat.progressive.app.native.ProgressiveNative
import org.json.JSONObject

class AlarmReceiver : BroadcastReceiver() {
    override fun onReceive(context: Context, intent: Intent) {
        val alarmJson = intent.getStringExtra("ALARM_JSON") ?: return
        try {
            val json = JSONObject(alarmJson)
            val alarmId = json.getString("id")
            val alarmNote = json.getString("note")
            val ttsDelay = json.optInt("ttsDelaySeconds", 5)
            val preAction = json.optString("preActionParam", "").ifEmpty { null }

            context.startActivity(
                AlarmActivity.newIntent(context, alarmId, alarmNote, ttsDelay, preAction)
            )
        } catch (_: Exception) { }
    }

    companion object {
        fun schedule(context: Context, alarmJson: String, triggerAtMs: Long) {
            val intent = Intent(context, AlarmReceiver::class.java).apply {
                putExtra("ALARM_JSON", alarmJson)
            }
            val pending = PendingIntent.getBroadcast(
                context,
                alarmJson.hashCode(),
                intent,
                PendingIntent.FLAG_UPDATE_CURRENT or PendingIntent.FLAG_IMMUTABLE
            )
            val alarmManager = context.getSystemService(Context.ALARM_SERVICE) as AlarmManager
            alarmManager.setExactAndAllowWhileIdle(
                AlarmManager.RTC_WAKEUP,
                triggerAtMs,
                pending
            )
        }

        fun cancel(context: Context, alarmHash: Int) {
            val intent = Intent(context, AlarmReceiver::class.java)
            val pending = PendingIntent.getBroadcast(
                context, alarmHash, intent,
                PendingIntent.FLAG_IMMUTABLE or PendingIntent.FLAG_NO_CREATE
            )
            pending?.cancel()
            (context.getSystemService(Context.ALARM_SERVICE) as AlarmManager).cancel(pending)
        }
    }
}
