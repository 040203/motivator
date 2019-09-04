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
  |> then_(_ =>
       Expo.Notifications.scheduleLocalNotificationAsync(
         {
           "title": "Update progress",
           "body": "It's time to udpate your progress for today",
         },
         {"time": date, "repeat": "day"},
       )
     )
  |> thenIgnore(ignore)
  |> catchIgnore(res => Js.log(res));
};