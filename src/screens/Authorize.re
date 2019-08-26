open ReactNative;
open ReactNative.Style;

type stage =
  | Greeting
  | Info;

type action =
  | SetStage(stage)
  | SetCurrentWeight(float)
  | SetGoal(float);

type state = {
  stage,
  currentWeight: float,
  goal: float,
};

let styles =
  Styles.makeStyles(theme =>
    {
      "container":
        style(~flex=1., ~alignItems=`center, ~backgroundColor="#F5FCFF", ()),
      "weightInput": style(~width=100.->pct),
      "image":
        style(
          ~marginTop=theme.spacing(4.),
          ~marginBottom=theme.spacing(4.),
          ~height=300.->dp,
          ~width=300.->dp,
          (),
        ),
      "description": style(~marginTop=auto, ()),
    }
  );

module GreetingStage = {
  [@react.component]
  let make = (~onContinue) => {
    <>
      <Image
        style=styles##image
        source={Image.Source.fromRequired(
          Packager.require("../../../../assets/logo.png"),
        )}
      />
      <StyledText variant=LargeTitle value="Hello" />
      <StyledText value="blablablalblablblal" />
      <LargeButton title="Continue" onPress=onContinue />
    </>;
  };
};

module InfoStage = {
  [@react.component]
  let make = (~state, ~dispatch) => {
    <>
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
      <StyledText style=styles##description value="blablablalblablblal" />
      <LargeButton title="Continue" onPress=Js.log />
    </>;
  };
};

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | SetStage(stage) => {...state, stage}
        | SetGoal(goal) => {...state, goal}
        | SetCurrentWeight(currentWeight) => {...state, currentWeight}
        },
      {stage: Greeting, currentWeight: 100., goal: 80.},
    );

  let selectStage = (newStage, _) => dispatch(SetStage(newStage));

  <SafeAreaView style=styles##container>
    {switch (state.stage) {
     | Greeting => <GreetingStage onContinue={selectStage(Info)} />
     | Info => <InfoStage state dispatch />
     }}
  </SafeAreaView>;
};