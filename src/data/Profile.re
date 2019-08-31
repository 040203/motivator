type profile = {
  name: string,
  goal: float,
};

type profileContextValue = {
  data: option(profile),
  updateProfile: profile => unit,
};

let profileContext = React.createContext((None, ignore));
let useProfile = () => React.useContext(profileContext);
let useProfileOrFail = () => {
  let (profile, updateProfile) = useProfile();

  switch (profile) {
  | Some(profile) => (profile, updateProfile)
  | None => raise(Not_found)
  };
};

module ProfileProvider = {
  let makeProps = Helpers.makeProviderProps
  let make = React.Context.provider(profileContext);
};

[@react.component]
let make = (~children) => {
  let (profile, setProfile) = React.useState(_ => None);

  React.useEffect0(() => {
    Db.getProfile(profile =>
      setProfile(_ =>
        switch (profile) {
        | None => None
        | Some(profileObj) =>
          Some({
            name: profileObj |> Db.nameGet,
            goal: profileObj |> Db.goalGet,
          })
        }
      )
    );

    None;
  });

  let updateProfile = data => {
    (data.name, data.goal)
    |> Db.insertProfile(_ =>
         setProfile(_ => Some({name: data.name, goal: data.goal}))
       );
  };

  <ProfileProvider value=(profile, updateProfile)>
    ...children
  </ProfileProvider>;
};