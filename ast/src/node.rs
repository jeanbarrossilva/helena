// Copyright © 2025 Jean Silva
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
//                            http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under
// the License.

use std::fmt::{Debug, Display, Formatter};
use std::sync::OnceLock;
use regex::Regex;

use crate::branch::Branch;

#[cfg(windows)]
const NEWLINE: &str = "\r\n";

#[cfg(not(windows))]
const NEWLINE: &str = "\n";

/// Error that occurs when the name of a node that is required to follow a specific pattern does not
/// match it.
#[derive(Debug)]
pub struct UnmatchedPatternError<'a> {
  /// The pattern not matched by the text.
  pattern: Regex,

  /// The text that does not match the pattern specified by the node.
  text: &'a str
}

impl Display for UnmatchedPatternError<'_> {
  fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
    Display::fmt(
      &format!(
        "Textual representation of node (\"{}\") does not match \"{}\".",
        self.text, self.pattern
      ),
      f
    )
  }
}

impl std::error::Error for UnmatchedPatternError<'_> {}

/// A node (token) in the AST.
pub trait Node: Debug {
  /// Obtains the vertical position of this node in the AST. Corresponds to the number of the line
  /// in which it is located in the source file.
  fn column(&self) -> u32;

  /// Obtains the horizontal position of this node in the AST. Corresponds to the index of the
  /// character that delimits the start of this node in the line of the source file in which it is
  /// located.
  fn row(&self) -> u32;

  /// Obtains the textual representation of this node in the AST.
  fn text(&self) -> Result<&str, UnmatchedPatternError>;

  /// Obtains the branch by which the nodes that are expected to follow this one are defined.
  fn branch(&self) -> &Branch<Self, impl Node> {
    static BRANCH: OnceLock<Branch<Self, impl Node>> = OnceLock::new();
    BRANCH.get_or_init(|| Branch::from(self))
  }
}

/// Node of an OS-specific linebreak.
#[derive(Debug)]
pub(crate) struct NewlineNode<'a, N: Node> {
  /// Vertical position of this node in the AST. Corresponds to the number of the line in which it
  /// is located in the source file.
  column: u32,

  /// Horizontal position of this node in the AST. Corresponds to the index of the character that
  /// delimits the start of this node in the line of the source file in which it is located.
  row: u32
}

impl<N> NewlineNode<'_, N> {
  /// Instantiates a newline node.
  fn new(column: u32, row: u32) -> Self {
    NewlineNode {
      column,
      row
    }
  }
}

impl<N: Node> Node for NewlineNode<N> {
  fn column(&self) -> u32 {
    self.column
  }

  fn row(&self) -> u32 {
    self.row
  }

  fn text(&self) -> Result<&str, UnmatchedPatternError> {
    Ok(NEWLINE)
  }
}

pub(crate) fn matching<'a>(
  pattern: Result<Regex, regex::Error>,
  text: &str
) -> Result<&'a str, UnmatchedPatternError> {
  let unwrapped_pattern = pattern.unwrap();
  if !unwrapped_pattern.is_match(text) {
    return Err(UnmatchedPatternError {
      pattern: unwrapped_pattern,
      text
    });
  }
  Ok(text)
}
