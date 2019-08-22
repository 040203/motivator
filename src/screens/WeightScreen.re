open Expo;
open ReactNative;
open ReactNative.Style;
open ReasonDateFns;

let str = React.string;

let styles =
  Styles.makeStyles(theme =>
    {
      "progress":
        style(
          ~marginTop=theme.spacing(2.),
          ~marginBottom=theme.spacing(7.),
          ~height=200.->dp,
          ~width=200.->dp,
          (),
        ),
      "progressLabelContainer":
        style(
          ~width=200.->dp,
          ~justifyContent=`center,
          ~alignItems=`center,
          (),
        ),
      "progressLabel": style(~fontSize=32., ()),
    }
  );

[@react.component]
let make = () => {
  let (currentWeight, setCurrentWight) = React.useState(() => 0.);
  let (lastWeightDate, setLastWeightDate) = React.useState(() => None);
  let (isAddRecordOpen, setIsAddRecordOpen) = React.useState(() => false);

  React.useEffect0(() => {
    Db.getLatestWieght(weight => {
      setCurrentWight(_ => weight |> Db.weightGet);
      setLastWeightDate(_ =>
        Some(
          weight |> Db.dateGet |> DateFns.parse(Js.Date.make(), "yyyy-MM-dd"),
        )
      );
    });

    None;
  });

  let toggleModal = _ => setIsAddRecordOpen(value => !value);
  let isEditing =
    switch (lastWeightDate) {
    | Some(date) => DateFns.isSameDay(date, Js.Date.make())
    | None => false
    };

  let handleWeightChange = newWieght => {
    setCurrentWight(_ => newWieght);
    setLastWeightDate(_ => Some(Js.Date.make()));
    toggleModal();

    (Js.Date.make(), newWieght) |> Db.insertWieght(ignore);
  };

  let addNewRecordTile = {
    <LargeTile
      icon="plus"
      text={js|Новое измерение веса|js}
      onPress=toggleModal
      colors=[|
        Styles.theme.palette.primaryLight,
        Styles.theme.palette.primaryDark,
      |]
    />;
  };

  let modifyRecordTile = {
    <LargeTile
      icon="plus"
      text={js|Изменить вес за сегоня|js}
      onPress=toggleModal
      colors=[|
        Styles.theme.palette.secondaryLight,
        Styles.theme.palette.secondaryDark,
      |]
    />;
  };

  if (currentWeight === 0.) {
    <> </>;
  } else {
    <>
      <SvgCharts.ProgressCircle
        style=styles##progress
        progress=0.2
        strokeWidth=10
        progressColor="rgb(134, 65, 244)"
        animate=true
        animationDuration=300>
        <View style=styles##progressLabelContainer>
          <Text style=styles##progressLabel>
            {str(currentWeight->Js.Float.toString ++ "kg")}
          </Text>
        </View>
      </SvgCharts.ProgressCircle>
      <StyledText
        variant=Title
        value={js|До цели осталось 30кг, вперед!|js}
      />
      {isEditing ? modifyRecordTile : addNewRecordTile}
      <WeightAddRecord
        isEditing
        initialWieght=currentWeight
        isOpen=isAddRecordOpen
        onClose=toggleModal
        onChange=handleWeightChange
      />
    </>;
  };
};