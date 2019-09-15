open ReasonDateFns;
open Expo.SQLite;
open Expo.SQLite.Transaction;

let db = openDatabase("weight.db");

[@bs.deriving abstract]
type weight = {
  date: string,
  dateTimeIso: string,
  weight: float,
};

type profile = {
  name: string,
  goal: float,
  initialWeight: float,
};

[@bs.deriving abstract]
type profileDb = {
  name: string,
  goal: float,
  initialWeight: float,
};

let sql = (sqlQuery, arguments) => {
  let argsNote =
    switch (arguments) {
    | [] => ""
    | list =>
      list
      |> List.map(item =>
           switch (item) {
           | `String(string) => string
           | `Float(float) => Js.Float.toString(float)
           }
         )
      |> String.concat(", ")
      |> (value => " (" ++ value ++ ")")
    };

  Js.log("Running sql: " ++ sqlQuery ++ argsNote);

  Js.Promise.make((~resolve, ~reject) =>
    Database.transaction(
      db,
      tx =>
        executeSql(
          tx,
          sqlQuery,
          arguments,
          (_tx, rs) => resolve(. rs |> rowsGet |> _arrayGet),
          (_tx, error) => Js.log(error),
        )
        |> ignore,
      _ => (),
      _ => (),
    )
    |> ignore
  );
};

let migrate = onSuccess =>
  Js.Promise.all2((
    "
    CREATE TABLE IF NOT EXISTS weights (
    	date date PRIMARY KEY NOT NULL,
      dateTimeIso text NOT NULL,
    	weight float
    );
    "
    ->sql([]),
    "
    CREATE TABLE IF NOT EXISTS profile (
      name text PRIMARY KEY NOT NULL,
      goal float NOT NULL,
      initialWeight float NOT NULL
    );
    "
    ->sql([]),
  ))
  |> Helpers.thenIgnore(onSuccess)
  |> ignore;

let getLatestWieght = (onSuccess: weight => unit) =>
  "SELECT * FROM weights ORDER BY date DESC LIMIT 1"->sql([])
  |> Helpers.thenIgnore(weights => onSuccess(weights[0]))
  |> ignore;

let selectAllWeights = (onSuccess: array(weight) => unit) =>
  sql("SELECT * FROM weights", []) |> Helpers.thenIgnore(onSuccess) |> ignore;

let insertWieght = (onSuccess, (date, weight): (Js.Date.t, float)) =>
  [
    `String(date |> DateFns.format("yyyy-MM-dd")),
    `String(date |> Js.Date.toISOString),
    `Float(weight),
  ]
  |> sql(
       "INSERT or REPLACE INTO weights (date, dateTimeIso, weight) values(?, ?, ?)",
     )
  |> Helpers.thenIgnore(onSuccess)
  |> ignore;

let getProfile = (onSuccess: option(profileDb) => unit) =>
  sql("SELECT * FROM profile LIMIT 1", [])
  |> Helpers.thenIgnore(profiles => onSuccess(profiles[0]))
  |> ignore;

let insertProfile = (onSuccess, (name, goal, initialWeight)) =>
  [`String(name), `Float(goal), `Float(initialWeight)]
  |> sql(
       "INSERT or REPLACE INTO profile (name, goal, initialWeight) values(?, ?, ?)",
     )
  |> Helpers.thenIgnore(onSuccess)
  |> ignore;

let dropDb = db => sql("DROP TABLE IF EXISTS" ++ " " ++ db, []);

dropDb("profile")