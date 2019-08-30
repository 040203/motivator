open ReactNative;
open ReactNative.Style;

type stage =
  | Info
  | Greeting;

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
      "weightInput": style(~width=100.->pct),
      "image":
        style(
          ~marginTop=theme.spacing(4.),
          ~marginBottom=theme.spacing(4.),
          ~height=250.->dp,
          ~width=250.->dp,
          (),
        ),
      "description": style(~width=90.->pct, ()),
      "pullButtom": style(~marginTop=auto, ()),
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
        style=styles##pullButtom
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
        style={Style.array([|styles##pullButtom, styles##description|])}
        value="Try to keep your goals realistic. Keep in mind that you can loose maximum 5-10kg without breaks."
      />
      <LargeButton title="Continue" onPress=onContinue />
    </SafeAreaView>;
  };
};

let animationFromStage = stage => {
  let margin =
    switch (stage) {
    | Greeting => 0.
    | Info => window##width
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
  let marginLeft = Animated.Value.create(0.);

  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | SetStage(stage) =>
          marginLeft
          ->Animated.timing(animationFromStage(stage))
          ->Animated.start(~endCallback=ignore, ());

          {...state, stage};

        | SetCurrentWeight(currentWeight) => {...state, currentWeight}
        | SetGoal(goal) => {...state, goal}
        },
      {stage: Greeting, currentWeight: 100., goal: 80.},
    );

  let selectStage = (newStage, _) => dispatch(SetStage(newStage));

  let submit = _ => {
    (Js.Date.make(), state.currentWeight)
    |> Db.insertWieght(_ => setProfile({name: "heh", goal: state.goal}));
  };

  <Animated.View
    style={Style.array([|
      styles##container,
      style(~left=marginLeft->Animated.StyleProp.float->dp, ()),
    |])}>
    <GreetingStage onContinue={Info |> selectStage} />
    <InfoStage state dispatch onContinue=submit />
  </Animated.View>;
};