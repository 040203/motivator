open Helpers;
open Js.Promise;

let checkPermission = () => {
  Expo.Permissions.getAsync(Expo.Permissions.Notifications)
  |> then_(res =>
       if (res##status !== "granted") {
         Expo.Permissions.askAsync(Expo.Permissions.Notifications);
       } else {
         resolve(ignore);
       }
     );
};

let scheduleReminder = date => {
  checkPermission()
  |> thenIgnore(_ => Expo.Notifications.cancelAllScheduledNotificationsAsync
  ())
  |> then_(_ =>
       Expo.Notifications.scheduleLocalNotificationAsync(
         { 
           "title": Localization.i18n("Notification_title"),
           "body": Localization.i18n("Notification_body"),
         },
         {"time": date, "repeat": "day"},
       )
     )
  |> thenIgnore(Js.log)
  |> catchIgnore(res => Js.log(res));
};
