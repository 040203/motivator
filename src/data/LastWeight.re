type weightData = {
  date: Js.Date.t,
  weight: float,
};

let lastWeightContext =
  React.createContext((
    None,
    (~onSuccess: unit => unit=ignore, newWeight: float) => (),
  ));

module LastWeightProvider = {
  let makeProps = Helpers.makeProviderProps;
  let make = React.Context.provider(lastWeightContext);
};

let useLastWeight = () => React.useContext(lastWeightContext);
let useLastWeightOrFail = () => {
  let (lastWeight, updateLastWeight) = useLastWeight();

  switch (lastWeight) {
  | Some(lastWeight) => (lastWeight, updateLastWeight)
  | None => raise(Not_found)
  };
};

[@react.component]
let make = (~children) => {
  let (lastWeight, setLastWeight) = React.useState(_ => None);

  React.useEffect0(() => {
    Db.getLatestWieght(weight =>
      setLastWeight(_ =>
        Some({
          weight: weight |> Db.weightGet,
          date: weight |> Db.dateTimeIsoGet |> Js.Date.fromString,
        })
      )
    )
    |> ignore;

    None;
  });

  let updateWeight = (~onSuccess=ignore, newWeight) => {
    let date = Js.Date.make();
    setLastWeight(_ => Some({date, weight: newWeight}));

    (date, newWeight) |> Db.insertWieght(_ => onSuccess());
  };

  <LastWeightProvider value=(lastWeight, updateWeight)>
    ...children
  </LastWeightProvider>;
};