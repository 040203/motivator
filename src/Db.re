open Js.Promise;
open ReasonDateFns;
open Expo.SQLite;
open Expo.SQLite.Transaction;

let db = openDatabase("weight.db");

[@bs.deriving abstract]
type weight = {
  date: string,
  weight: float,
};

[@bs.deriving abstract]
type profile = {
  goal: float,
  name: string,
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

let resolveOnSuccess = onSuccess =>
  then_(result => {
    onSuccess(result);
    resolve(ignore);
  });

let migrate = onSuccess =>
  Js.Promise.all2((
    "
    CREATE TABLE IF NOT EXISTS weights (
    	date date PRIMARY KEY NOT NULL,
    	weight float
    );
    "
    ->sql([]),
    "
    CREATE TABLE IF NOT EXISTS profile (
      name text PRIMARY KEY NOT NULL,
      goal float NOT NULL
    );
    "
    ->sql([]),
  ))
  |> resolveOnSuccess(onSuccess)
  |> ignore;

let getLatestWieght = (onSuccess: weight => unit) =>
  "SELECT * FROM weights ORDER BY date DESC LIMIT 1"->sql([])
  |> resolveOnSuccess(weights => onSuccess(weights[0]))
  |> ignore;

let selectAllWeights = (onSuccess: array(weight) => unit) =>
  sql("SELECT * FROM weights", []) |> resolveOnSuccess(onSuccess) |> ignore;

let insertWieght = (onSuccess, (date, weight): (Js.Date.t, float)) =>
  [`String(date |> DateFns.format("yyyy-MM-dd")), `Float(weight)]
  |> sql("INSERT or REPLACE INTO weights (date, weight) values(?, ?)")
  |> resolveOnSuccess(onSuccess)
  |> ignore;

let getProfile = (onSuccess: option(profile) => unit) =>
  sql("SELECT * FROM profile LIMIT 1", [])
  |> resolveOnSuccess(profiles => onSuccess(profiles[0]))
  |> ignore;

let insertProfile = (onSuccess, (name, goal)) =>
  [`String(name), `Float(goal)]
  |> sql("INSERT or REPLACE INTO profile (name, goal) values(?, ?)")
  |> resolveOnSuccess(onSuccess)
  |> ignore;

let dropDb = db => sql("DROP TABLE IF EXISTS" ++ " " ++ db, []);