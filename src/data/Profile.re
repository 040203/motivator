type profile = {
  name: string,
  goal: float,
};

type profileContextValue = {
  data: option(profile),
  updateProfile: profile => unit,
};

module ProfileContext = {
  let profileContext = React.createContext((None, ignore));

  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };

  let make = React.Context.provider(profileContext);
};

let useProfile = () => React.useContext(ProfileContext.profileContext);

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
    setProfile(_ => Some(data));
    (data.name, data.goal) |> Db.insertProfile(ignore);
  };

  <ProfileContext value=(profile, updateProfile)>
    ...children
  </ProfileContext>;
};