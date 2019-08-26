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
  let (profile, _) = Profile.useProfile();

  let (currentWieght, updateWieght) = Data.useLastWeight();
  let (isAddRecordOpen, setIsAddRecordOpen) = React.useState(() => false);

  let toggleModal = _ => setIsAddRecordOpen(value => !value);
  let isEditing =
    switch (currentWieght) {
    | Some({date}) => DateFns.isSameDay(date, Js.Date.make())
    | None => false
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

  let handleWeightChange = newWeight => {
    newWeight |> updateWieght;
    toggleModal();
  };

  switch (currentWieght) {
  | None => <> </>
  | Some({weight}) =>
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
            {str(weight->Js.Float.toString ++ "kg")}
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
        initialWieght=weight
        isOpen=isAddRecordOpen
        onClose=toggleModal
        onChange=handleWeightChange
      />
    </>
  };
};