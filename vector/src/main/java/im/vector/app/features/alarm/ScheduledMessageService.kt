package im.vector.app.features.alarm

import android.app.AlarmManager
import android.app.PendingIntent
import android.app.Service
import android.content.Context
import android.content.Intent
import android.os.IBinder
import chat.progressive.app.native.ProgressiveNative

class ScheduledMessageService : Service() {
    override fun onBind(intent: Intent?): IBinder? = null

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        // Check for due messages and schedule next check
        try {
            ProgressiveNative.ensureLoaded()
            val pending = ProgressiveNative.nativeSchedGetPending()
            if (pending.isNotEmpty() && pending != "[]") {
                // Messages are pending — they'll be sent when the room is open
                // Just reschedule this check
            }
        } catch (_: Exception) { }
        
        // Reschedule self in 60 seconds
        val alarmManager = getSystemService(Context.ALARM_SERVICE) as AlarmManager
        val intent = Intent(this, ScheduledMessageService::class.java)
        val pending = PendingIntent.getService(this, 0, intent, PendingIntent.FLAG_IMMUTABLE)
        alarmManager.setExactAndAllowWhileIdle(
            AlarmManager.RTC_WAKEUP,
            System.currentTimeMillis() + 60000,
            pending
        )
        
        stopSelf()
        return START_NOT_STICKY
    }
}
