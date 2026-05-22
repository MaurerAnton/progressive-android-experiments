package chat.progressive.app.features.alarm

import android.app.AlarmManager
import android.app.PendingIntent
import android.app.Service
import android.content.Context
import android.content.Intent
import android.os.IBinder
import chat.progressive.app.native.ProgressiveNative
import org.json.JSONArray

class AlarmSchedulerService : Service() {
    override fun onBind(intent: Intent?): IBinder? = null

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        try {
            ProgressiveNative.ensureLoaded()
            val json = ProgressiveNative.nativeAlarmListAll()
            val alarms = JSONArray(json)
            val alarmManager = getSystemService(Context.ALARM_SERVICE) as AlarmManager

            for (i in 0 until alarms.length()) {
                val alarm = alarms.getJSONObject(i)
                val enabled = alarm.optBoolean("enabled", true)
                val triggerMs = alarm.optLong("triggerAtMs", 0)
                val id = alarm.getString("id")
                if (enabled && triggerMs > System.currentTimeMillis()) {
                    AlarmReceiver.schedule(this, alarm.toString(), triggerMs)
                }
            }
        } catch (_: Exception) { }

        // Reschedule self in 5 minutes
        val nextIntent = Intent(this, AlarmSchedulerService::class.java)
        val pending = PendingIntent.getService(this, 0, nextIntent,
            PendingIntent.FLAG_IMMUTABLE or PendingIntent.FLAG_UPDATE_CURRENT)
        val am = getSystemService(Context.ALARM_SERVICE) as AlarmManager
        am.setExactAndAllowWhileIdle(AlarmManager.RTC_WAKEUP,
            System.currentTimeMillis() + 300000, pending)

        stopSelf()
        return START_NOT_STICKY
    }

    companion object {
        fun startScheduling(context: Context) {
            val intent = Intent(context, AlarmSchedulerService::class.java)
            context.startService(intent)
        }
    }
}
