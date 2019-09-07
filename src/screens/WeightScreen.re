open ReactNative;
open ReactNative.Style;
open ReasonDateFns;

let str = React.string;

let styles =
  Styles.makeStyles(theme =>
    {
      "subtitle":
        style(
          ~marginLeft=theme.spacing(2.),
          ~marginRight=theme.spacing(2.),
          ~marginBottom=theme.spacing(2.),
          (),
        ),
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
    }
  );

[@react.component]
let make = () => {
  let prevProgressRef = React.useRef(0.);
  let (profile, _) = Profile.useProfileOrFail();
  let (currentWieght, updateWieght) = LastWeight.useLastWeightOrFail();
  let (isAddRecordOpen, setIsAddRecordOpen) = React.useState(() => false);

  let toTheGoal =
    React.useMemo2(
      () => currentWieght.weight -. profile.goal,
      (currentWieght, profile.goal),
    );

  let progress =
    React.useMemo3(
      () => {
        let wantToLose = profile.initialWeight -. profile.goal;
        let progress = 1. -. toTheGoal /. wantToLose;

        progress <= 0. ? 0.01 : progress;
      },
      (toTheGoal, profile.goal, profile.initialWeight),
    );

  // animation so weird if animating long change like from 0.1 to 0.6
  let shouldAnimateProgress =
    React.useMemo1(
      _ =>
        toTheGoal > 0.
        && Js.Math.abs_float(React.Ref.current(prevProgressRef) -. progress)
        < 0.1,
      [|progress|],
    );

  React.Ref.setCurrent(prevProgressRef, progress);

  let toggleModal = _ => setIsAddRecordOpen(value => !value);
  let isEditing = DateFns.isSameDay(currentWieght.date, Js.Date.make());

  let handleWeightChange = newWeight => {
    newWeight |> updateWieght;
    toggleModal();
    Expo.Haptics.notificationAsync(Expo.Haptics.Success);
  };

  <>
    <StyledText
      size=18.
      textAlign=`center
      style=styles##subtitle
      i18n="Last weigh was"
      interpolate={
        "date":
          currentWieght.date
          |> Localization.Date.localizedFormatDistance(Js.Date.make()),
      }
    />
    <SvgCharts.ProgressCircle
      progress
      strokeWidth=10
      style=styles##progress
      progressColor="rgb(134, 65, 244)"
      animate=shouldAnimateProgress
      animationDuration=500>
      <View style=styles##progressLabelContainer>
        <StyledText
          size=32.
          i18n="kg"
          interpolate={"value": currentWieght.weight}
        />
      </View>
    </SvgCharts.ProgressCircle>
    <StyledText
      variant=Title
      i18n="To the goal"
      interpolate={
        "toTheGoal": toTheGoal |> Js.Float.toFixedWithPrecision(~digits=1),
      }
    />
    <LargeTile
      onPress=toggleModal
      icon={isEditing ? "pen" : "plus"}
      i18n={isEditing ? "Change today weight record" : "Add new weight record"}
      colors=[|
        Styles.theme.palette.primaryLight,
        Styles.theme.palette.primaryDark,
      |]
    />
    <WeightAddRecord
      isEditing
      initialWieght={currentWieght.weight}
      isOpen=isAddRecordOpen
      onClose=toggleModal
      onChange=handleWeightChange
    />
  </>;
};