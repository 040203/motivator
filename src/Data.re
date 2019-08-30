open ReasonDateFns;

type weightData = {
  date: Js.Date.t,
  weight: float,
};

let useLastWeight = () => {
  let (lastWeight, setLastWeight) = React.useState(_ => None);

  React.useEffect0(() => {
    Db.getLatestWieght(weight =>
      setLastWeight(_ =>
        Some({
          weight: weight |> Db.weightGet,
          date:
            weight
            |> Db.dateGet
            |> DateFns.parse(Js.Date.make(), "yyyy-MM-dd"),
        })
      )
    )
    |> ignore;

    None;
  });

  let updateWeight = newWeight => {
    let date = Js.Date.make();
    setLastWeight(_ => Some({date, weight: newWeight}));

    (date, newWeight) |> Db.insertWieght(ignore);
  };

  (lastWeight, updateWeight);
};