open ReactNative;
open ReactNative.Style;
open Helpers;
open ReasonDateFns;

type stage =
  | Info
  | Greeting
  | Notification;

type action =
  | SetGoal(float)
  | SetStage(stage)
  | SetCurrentWeight(float);

type state = {
  stage,
  goal: float,
  currentWeight: float,
};

let window = Dimensions.get(`window);

let styles =
  Styles.makeStyles(theme =>
    {
      "container": style(~flex=1., ~flexDirection=`row, ()),
      "stageContainer":
        style(
          ~width=window##width->dp,
          ~height=100.->pct,
          ~flexDirection=`column,
          ~alignItems=`center,
          (),
        ),
      "fullWidth": style(~width=100.->pct, ()),
      "image":
        style(
          ~marginTop=theme.spacing(4.),
          ~marginBottom=theme.spacing(4.),
          ~height=250.->dp,
          ~width=250.->dp,
          (),
        ),
      "description": style(~width=90.->pct, ()),
      "pullBottom": style(~marginTop=auto, ()),
    }
  );

module GreetingStage = {
  [@react.component]
  let make = (~onContinue) => {
    <SafeAreaView style=styles##stageContainer>
      <Image
        style=styles##image
        source={Image.Source.fromRequired(
          Packager.require("../../../../assets/logo.png"),
        )}
      />
      <StyledText variant=LargeTitle value="Hello" />
      <StyledText
        variant=Title
        textAlign=`center
        style=styles##description
        value="I am your fitness unicorn. I'll try to help you with your goals, but everything depends only on you. Let's start!"
      />
      <LargeButton
        style=styles##pullBottom
        title="Continue"
        onPress=onContinue
      />
    </SafeAreaView>;
  };
};

module InfoStage = {
  [@react.component]
  let make = (~state, ~dispatch, ~onContinue) => {
    <SafeAreaView style=styles##stageContainer>
      <StyledText variant=LargeTitle value="Current weight" />
      <WeightInput
        value={state.currentWeight}
        onChange={newValue => dispatch(SetCurrentWeight(newValue))}
      />
      <StyledText variant=LargeTitle value="Goal" />
      <WeightInput
        value={state.goal}
        onChange={newValue => dispatch(SetGoal(newValue))}
      />
      <StyledText
        textAlign=`center
        style={Style.array([|styles##pullBottom, styles##description|])}
        value="Try to keep your goals realistic. Keep in mind that you can loose maximum 5-10kg without breaks."
      />
      <LargeButton title="Continue" onPress=onContinue />
    </SafeAreaView>;
  };
};

module NotificaitonStage = {
  [@react.component]
  let make = (~onContinue) => {
    let (date, setDate) =
      React.useState(_ =>
        Js.Date.make()
        |> DateFns.addDays(1)
        |> DateFns.startOfDay
        |> DateFns.setHours(7.)
      );

    let setupNotification = _ => {
      NotificationManager.scheduleReminder()
      |> thenIgnore(_ => onContinue())
      |> catchIgnore(_ => onContinue())
      |> ignore;
    };

    <SafeAreaView style=styles##stageContainer>
      <StyledText variant=LargeTitle value="Reminder" />
      <StyledText
        variant=Title
        style=styles##description
        value="It is required to fill the progress each day. We can send you notification whenever you want to remind you about weighing."
      />
      <DatePickerIOS
        date
        mode=`time
        minuteInterval=`_10
        onDateChange={date => setDate(_ => date)}
        locale=Expo.Localization.locale
        style=styles##fullWidth
      />
      <LargeButton
        style=styles##pullBottom
        title="Setup reminder"
        onPress=setupNotification
      />
      <Button title="Without reminder" onPress={_ => onContinue()} />
    </SafeAreaView>;
  };
};

let animationFromStage = stage => {
  let margin =
    switch (stage) {
    | Greeting => 0.
    | Info => window##width
    | Notification => window##width *. 2.
    };

  Animated.Value.Timing.config(
    ~toValue=Animated.Value.Timing.fromRawValue(-. margin),
    ~duration=300.,
    (),
  );
};

[@react.component]
let make = () => {
  let (_, setProfile) = Profile.useProfile();
  let (_, setLastWeight) = LastWeight.useLastWeight();
  let marginLeft =
    React.useRef(Animated.Value.create(0.)) |> React.Ref.current;

  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | SetGoal(goal) => {...state, goal}
        | SetCurrentWeight(currentWeight) => {...state, currentWeight}
        | SetStage(stage) =>
          marginLeft
          ->Animated.timing(stage |> animationFromStage)
          ->Animated.start(~endCallback=ignore, ());

          {...state, stage};
        },
      {stage: Greeting, currentWeight: 100., goal: 90.},
    );

  let selectStage = (newStage, _) => dispatch(SetStage(newStage));

  let submit = _ => {
    let insertProfile = _ =>
      setProfile({
        name: "some name",
        goal: state.goal,
        initialWeight: state.currentWeight,
      });

    setLastWeight(state.currentWeight, ~onSuccess=insertProfile);
  };

  <Animated.View
    style={Style.array([|
      styles##container,
      style(~left=marginLeft->Animated.StyleProp.float->dp, ()),
    |])}>
    <GreetingStage onContinue={Info |> selectStage} />
    <InfoStage state dispatch onContinue={Notification |> selectStage} />
    <NotificaitonStage onContinue=submit />
  </Animated.View>;
};