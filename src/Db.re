open ReasonDateFns;
open Expo.SQLite;
open Expo.SQLite.Transaction;

let db = openDatabase("weight.db");

[@bs.deriving abstract]
type weight = {
  date: string,
  weight: float,
};

let sql = (sqlQuery, onSuccess, arguments) => {
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

  Database.transaction(
    db,
    tx =>
      executeSql(
        tx,
        sqlQuery,
        arguments,
        (_tx, rs) => onSuccess(rs |> rowsGet |> _arrayGet),
        (_tx, error) => Js.log(error),
      )
      |> ignore,
    _ => (),
    _ => (),
  )
  |> ignore;
};

let migrate = onSuccess =>
  sql(
    "
    CREATE TABLE IF NOT EXISTS weights (
    	date date PRIMARY KEY NOT NULL,
    	weight float
    );
    ",
    onSuccess,
    [],
  );

let getLatestWieght = (onSuccess: weight => unit) =>
  "SELECT * FROM weights ORDER BY date DESC LIMIT 1"->sql(wieghts => onSuccess(wieghts[0]), []);

let selectAllWeights = (onSuccess: array(weight) => unit) =>
  sql("SELECT * FROM weights", onSuccess, []);

let insertWieght = (onSuccess, (date, weight): (Js.Date.t, float)) =>
  [`String(date |> DateFns.format("yyyy-MM-dd")), `Float(weight)]
  |> sql("INSERT or REPLACE INTO weights (date, weight) values(?, ?)", onSuccess);
